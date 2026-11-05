// cuda-nvcc's lib{nvcc} is binless: it carries the headers and the compile
// options that come with the compiler. Importing it is the whole check.

#include <fatbinary_section.h>

#include <cstdio>

int
main ()
{
  std::printf ("nvcc: fatbin wrapper magic %#x\n", FATBINC_MAGIC);
}
