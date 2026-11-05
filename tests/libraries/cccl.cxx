// cuda-cccl is header only, so the check is at compile time.

#include <cuda/std/array>

#include <cstdio>

int
main ()
{
  constexpr cuda::std::array<int, 3> a {1, 2, 3};
  static_assert (a.size () == 3, "cccl");
  std::printf ("cccl: cuda::std::array of %d\n", static_cast<int> (a.size ()));
}
