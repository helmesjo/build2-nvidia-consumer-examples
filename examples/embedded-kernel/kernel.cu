// Compiled to a fatbin by the packaged nvcc, embedded into a header by the
// packaged bin2c, then loaded through the driver API.
//
// The include resolves only if the -I set harvested from the imported
// libraries actually reaches nvcc.

#include <cuda_runtime.h>

extern "C" __global__ void scale (float* p, float k, int n)
{
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < n)
    p[i] *= k;
}
