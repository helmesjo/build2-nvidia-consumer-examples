// The version-compatible plugin library exports only getCreators and
// setLoggerFinder, and no public header declares them, so the check declares
// the entry point itself and takes its address.

#include <cstdint>
#include <cstdio>

extern "C" void* const* getCreators (int32_t&);

int
main ()
{
  if (&getCreators == nullptr)
    return 1;

  std::printf ("nvinfer_vc_plugin: getCreators resolved\n");
}
