#include <cublasLt.h>

#include <cstddef>
#include <cstdio>

int
main ()
{
  std::size_t v (cublasLtGetVersion ());
  if (v == 0)
    return 1;

  std::printf ("cublasLt: %zu\n", v);
}
