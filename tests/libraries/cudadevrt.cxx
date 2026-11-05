// cudadevrt is the device-side runtime: its entry points are callable from
// device code only, so there is nothing a host program can reference.
// Linking it is the whole check.

#include <cstdio>

int
main ()
{
  std::printf ("cudadevrt: linked\n");
}
