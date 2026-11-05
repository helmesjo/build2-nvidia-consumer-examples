#include <NvOnnxParser.h>

#include <cstdio>

int
main ()
{
  int v (getNvOnnxParserVersion ());
  if (v == 0)
    return 1;

  std::printf ("nvonnxparser: %d\n", v);
}
