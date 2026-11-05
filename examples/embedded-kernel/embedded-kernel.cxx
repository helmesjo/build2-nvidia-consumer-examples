// The packaged nvcc compiles kernel.cu to a fatbin and the packaged bin2c
// embeds it into a header, both at build time. This program then loads that
// fatbin through the driver API and launches the kernel in it.

#include <cuda.h>         // driver API, from cuda-cudart
#include <cuda_runtime.h> // cuda-cudart

#include <examples/embedded-kernel/kernel_fatbin.hxx> // nvcc -> bin2c

#ifdef NDEBUG
#  undef NDEBUG
#endif

#include <cassert>
#include <cstdio>

int
main ()
{
  std::printf ("fatbin embedded by bin2c: %u bytes\n",
               static_cast<unsigned> (kernel_fatbinLength));
  assert (kernel_fatbinLength != 0);

  int count (0);
  if (cudaGetDeviceCount (&count) != cudaSuccess || count == 0)
  {
    std::printf ("no device, skipping the launch\n");
    return 0;
  }

  assert (cuInit (0) == CUDA_SUCCESS);

  CUdevice dev;
  assert (cuDeviceGet (&dev, 0) == CUDA_SUCCESS);

  char name[128] {};
  assert (cuDeviceGetName (name, sizeof (name), dev) == CUDA_SUCCESS);
  std::printf ("device: %s\n", name);

  // The primary context rather than cuCtxCreate, whose signature is versioned
  // (_v4 in CUDA 13, taking CUctxCreateParams). The primary context is also
  // the one cudart binds, so both APIs share it.
  //
  CUcontext ctx;
  assert (cuDevicePrimaryCtxRetain (&ctx, dev) == CUDA_SUCCESS);
  assert (cuCtxSetCurrent (ctx) == CUDA_SUCCESS);

  CUmodule mod;
  assert (cuModuleLoadData (&mod, kernel_fatbin) == CUDA_SUCCESS);
  CUfunction fn;
  assert (cuModuleGetFunction (&fn, mod, "scale") == CUDA_SUCCESS);

  const int n (4);
  float host[n] {1.0f, 2.0f, 3.0f, 4.0f};

  CUdeviceptr p;
  assert (cuMemAlloc (&p, sizeof (host)) == CUDA_SUCCESS);
  assert (cuMemcpyHtoD (p, host, sizeof (host)) == CUDA_SUCCESS);

  float k (2.5f);
  int len (n);
  void* args[] {&p, &k, &len};
  assert (cuLaunchKernel (fn, 1, 1, 1, n, 1, 1, 0, nullptr, args, nullptr) ==
          CUDA_SUCCESS);
  assert (cuCtxSynchronize () == CUDA_SUCCESS);
  assert (cuMemcpyDtoH (host, p, sizeof (host)) == CUDA_SUCCESS);

  std::printf ("scale(x2.5): %.1f %.1f %.1f %.1f\n",
               host[0], host[1], host[2], host[3]);
  assert (host[0] == 2.5f && host[3] == 10.0f);

  cuMemFree (p);
  cuModuleUnload (mod);
  cuDevicePrimaryCtxRelease (dev);
}
