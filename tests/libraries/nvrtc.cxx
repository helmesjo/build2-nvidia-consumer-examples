#include <nvrtc.h>

#include <cstddef>
#include <cstdio>

int
main ()
{
  int major (0), minor (0);
  if (nvrtcVersion (&major, &minor) != NVRTC_SUCCESS)
    return 1;

  // Compiling is what forces the bare-name load of nvrtc-builtins.
  //
  const char* src ("__global__ void k (float* p) { p[0] = 1.0f; }\n");

  nvrtcProgram prog;
  if (nvrtcCreateProgram (&prog, src, "k.cu", 0, nullptr, nullptr) != NVRTC_SUCCESS ||
      nvrtcCompileProgram (prog, 0, nullptr) != NVRTC_SUCCESS)
    return 1;

  std::size_t n (0);
  if (nvrtcGetPTXSize (prog, &n) != NVRTC_SUCCESS || n == 0)
    return 1;

  nvrtcDestroyProgram (&prog);

  std::printf ("nvrtc: v%d.%d, compiled %zu bytes of PTX\n", major, minor, n);
}
