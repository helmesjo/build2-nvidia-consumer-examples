#include <cublas_v2.h>

#include <cstdio>

int
main ()
{
  // The property query needs neither a handle nor a device.
  //
  int major (0), minor (0);
  if (cublasGetProperty (MAJOR_VERSION, &major) != CUBLAS_STATUS_SUCCESS ||
      cublasGetProperty (MINOR_VERSION, &minor) != CUBLAS_STATUS_SUCCESS)
    return 1;

  std::printf ("cublas: v%d.%d\n", major, minor);
}
