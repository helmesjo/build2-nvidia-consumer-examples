#include <cuda_runtime.h>

#include <cstdio>
#include <cstdlib>

int
main ()
{
  int v (0);
  if (cudaRuntimeGetVersion (&v) != cudaSuccess || v == 0)
    return 1;

  std::printf ("cudart: runtime %d\n", v);
}
