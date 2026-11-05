// Builds a small network with TensorRT, serializes it, deserializes it and
// runs it on the GPU.

#include <cuda_runtime.h> // cuda-cudart
#include <NvInfer.h>      // libtensorrt
#include <NvInferPlugin.h>

#ifdef NDEBUG
#  undef NDEBUG
#endif

#include <cassert>
#include <cstdio>
#include <memory>

namespace
{
  class logger: public nvinfer1::ILogger
  {
    void
    log (Severity s, const char* m) noexcept override
    {
      if (s <= Severity::kWARNING)
        std::printf ("  trt: %s\n", m);
    }
  };
}

int
main ()
{
  std::printf ("tensorrt %d\n", getInferLibVersion ());

  int count (0);
  if (cudaGetDeviceCount (&count) != cudaSuccess || count == 0)
  {
    std::printf ("no device, skipping the inference\n");
    return 0;
  }

  logger log;

  // Registers the bundled plugins. Not used by the network below, but this is
  // the call that proves nvinfer_plugin is linked and loadable.
  //
  assert (initLibNvInferPlugins (&log, ""));

  const int n (4);

  std::unique_ptr<nvinfer1::IHostMemory> plan;
  {
    std::unique_ptr<nvinfer1::IBuilder> b (nvinfer1::createInferBuilder (log));
    assert (b);
    std::unique_ptr<nvinfer1::INetworkDefinition> net (b->createNetworkV2 (0));
    assert (net);

    nvinfer1::ITensor* in (
      net->addInput ("in", nvinfer1::DataType::kFLOAT,
                     nvinfer1::Dims4 {1, 1, 1, n}));
    assert (in != nullptr);

    nvinfer1::IActivationLayer* relu (
      net->addActivation (*in, nvinfer1::ActivationType::kRELU));
    assert (relu != nullptr);

    relu->getOutput (0)->setName ("out");
    net->markOutput (*relu->getOutput (0));

    std::unique_ptr<nvinfer1::IBuilderConfig> cfg (b->createBuilderConfig ());
    assert (cfg);

    // Building an engine is what forces the bare-name load of
    // nvinfer_builder_resource_<arch>.
    //
    plan.reset (b->buildSerializedNetwork (*net, *cfg));
    assert (plan && plan->size () != 0);
  }
  std::printf ("serialized engine: %zu bytes\n", plan->size ());

  std::unique_ptr<nvinfer1::IRuntime> rt (nvinfer1::createInferRuntime (log));
  assert (rt);
  std::unique_ptr<nvinfer1::ICudaEngine> eng (
    rt->deserializeCudaEngine (plan->data (), plan->size ()));
  assert (eng);
  std::unique_ptr<nvinfer1::IExecutionContext> ctx (
    eng->createExecutionContext ());
  assert (ctx);

  float host[n] {-1.0f, 2.0f, -3.0f, 4.0f};

  void* din (nullptr);
  void* dout (nullptr);
  assert (cudaMalloc (&din, sizeof (host)) == cudaSuccess);
  assert (cudaMalloc (&dout, sizeof (host)) == cudaSuccess);
  assert (cudaMemcpy (din, host, sizeof (host), cudaMemcpyHostToDevice) ==
          cudaSuccess);

  assert (ctx->setTensorAddress ("in", din));
  assert (ctx->setTensorAddress ("out", dout));

  cudaStream_t s;
  assert (cudaStreamCreate (&s) == cudaSuccess);
  assert (ctx->enqueueV3 (s));
  assert (cudaStreamSynchronize (s) == cudaSuccess);
  assert (cudaMemcpy (host, dout, sizeof (host), cudaMemcpyDeviceToHost) ==
          cudaSuccess);

  std::printf ("relu(-1 2 -3 4): %.1f %.1f %.1f %.1f\n",
               host[0], host[1], host[2], host[3]);
  assert (host[0] == 0.0f && host[1] == 2.0f && host[3] == 4.0f);

  cudaStreamDestroy (s);
  cudaFree (din);
  cudaFree (dout);
}
