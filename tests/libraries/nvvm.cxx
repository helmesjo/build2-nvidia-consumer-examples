#include <nvvm.h>

#include <cstdio>

int
main ()
{
  int major (0), minor (0);
  if (nvvmVersion (&major, &minor) != NVVM_SUCCESS)
    return 1;

  std::printf ("nvvm: v%d.%d\n", major, minor);
}
