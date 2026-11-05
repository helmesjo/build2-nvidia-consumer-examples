// cuda-crt is binless and ships only crt/* headers, which refuse to be
// included directly. Importing it is the whole check.

#include <cstdio>

int
main ()
{
  std::printf ("crt: imported\n");
}
