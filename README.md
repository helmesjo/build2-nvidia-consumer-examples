# nvidia-consumer-examples

A build2 project that consumes every target exported by
[nvidia-cuda-toolkit](https://github.com/build2-packaging/nvidia-cuda-toolkit)
and [nvidia-tensorrt](https://github.com/build2-packaging/nvidia-tensorrt).
It is an example of how to depend on those packages, and a consumer-side
regression test for them.

The packages are resolved from `pkg.cppget.org/1/queue/testing` until they are
approved for `cppget.org` proper.

Written with AI assistance (Claude), and verified end to end on Windows with
MSVC and on Linux with GCC.


## The examples

Each is a self-contained program under `examples/`:

- **`embedded-kernel`.** The packaged `nvcc` compiles `kernel.cu` to a fatbin
  and the packaged `bin2c` embeds it into a header, both at build time. The
  program then loads it through the driver API and launches it.
- **`tensorrt-inference`.** A small network is built, serialized,
  deserialized and run.

Both need an NVIDIA GPU and skip the device-side part with a message when
there is none.


## What the tests cover

`tests/libraries` builds one executable per exported library, each linking
exactly that library and calling into it, so a failure names the library that
broke. `tests/executables` runs each exported executable through a testscript.

|  | Windows | Linux |
|---|---|---|
| libraries | 16 | 17 |
| executables | 9 | 10 |

`lib{tensorrt_shim}` and `exe{tensorrt_player}` are built on Linux only.


## What it proves

On both Windows with MSVC and Linux with GCC:

- Every exported target is consumable. All 17 libraries and 10 executables
  import, link, load and run from a plain `bdep init` against the published
  queue.
- The packages install correctly from a `bpkg` configuration, the class of
  defect that shipped as `cuda-cudart 13.3.1+3`.
- `lib{}` is the correct import spelling, not `libs{}`: `cudart` and
  `cudadevrt` are static on Windows.
- The toolchain works as a build toolchain, not just as files on disk. A
  broken `-I` harvest fails the build rather than passing silently.
- Platform differences are declared rather than hidden: the Linux-only
  targets stay in the build graph and in the `dist` archive on both
  platforms, gated with `include`.

It proves nothing about installing this project itself, or about CI. It needs
an NVIDIA GPU and multi-gigabyte downloads, so `builds: none`.


## Building

```
bdep init -C ../nvidia-consumer-examples-gcc @gcc cc config.cxx=g++
bdep test
```

Expect several gigabytes of downloads the first time: the dependencies fetch
the upstream CUDA and TensorRT archives. `config.<package>.cache` points a
package at an existing download directory if you already have one.
