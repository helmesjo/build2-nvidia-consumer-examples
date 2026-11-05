#include <NvInferPlugin.h>
#include <NvInferRuntime.h>

#include <cstdio>

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
  logger log;
  if (!initLibNvInferPlugins (&log, ""))
    return 1;

  std::printf ("nvinfer_plugin: registered\n");
}
