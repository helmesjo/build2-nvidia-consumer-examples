// nvblas interposes the Fortran BLAS entry points rather than offering an
// API of its own, so the check references one of them.

#include <cstdio>

extern "C" void sgemm_ (const char*, const char*,
                        const int*, const int*, const int*,
                        const float*, const float*, const int*,
                        const float*, const int*,
                        const float*, float*, const int*);

int
main ()
{
  if (&sgemm_ == nullptr)
    return 1;

  std::printf ("nvblas: sgemm_ resolved\n");
}
