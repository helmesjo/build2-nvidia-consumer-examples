// The full runtime, the lean and dispatch runtimes and the shim all export
// the same entry points, which is why each gets its own executable: they
// cannot share a link line.

#include <NvInferRuntime.h>

#include <cstdio>

int
main ()
{
  int32_t v (getInferLibVersion ());
  if (v == 0)
    return 1;

  std::printf ("nvinfer_dispatch: %d\n", v);
}
