# RStudio Server on OpenBSD

This branch adds experimental support for building and running RStudio Server on OpenBSD.

## Tested configuration

The port has been tested successfully with:

- OpenBSD 8.0 amd64
- RStudio 2026.10.999-dev+999 "Blue Mistflower"
- R 4.6.1
- LLVM/Clang from the OpenBSD base system
- Node.js 22
- Go 1.26
- Java 17

## Status

The following components have been built and tested successfully:

- `rserver`
- `rsession`
- `rserver-pam`
- `rpostback`
- RStudio Web UI
- interactive R sessions

## Branch

The tested branch is:

`openbsd-rstudio-2026.10`

## Build helper

The reproducible OpenBSD build and setup scripts are maintained in the companion repository:

https://github.com/Lars-Mehnen/rstudio-server-openbsd

The helper repository contains scripts for:

- preparing OpenBSD dependencies
- building native OpenBSD `esbuild` binaries
- building SOCI
- configuring and building RStudio Server
- preparing the runtime tree
- starting a development RStudio Server instance

## Upgrading R

After a system R upgrade (e.g. from 4.5.x to 4.6.x) `rsession` must be relinked because
the versioned `libR.so` ABI number changes. Clear the CMake cache and rebuild:

```sh
rm build/CMakeCache.txt
cmake -B build ...   # your usual configure flags
cmake --build build --target rsession
```

## Notes

This is an experimental OpenBSD port and is not an officially supported Posit platform.

The current port includes OpenBSD-specific adaptations for process handling, resource detection, file monitoring, PAM include handling, Boost extraction, RapidJSON compatibility, Panmirror/esbuild, and related build-system differences.

The vendored {fmt} library is patched at configure time to add the missing `<stdlib.h>`
include required by OpenBSD's Clang (see `src/cpp/ext/CMakeLists.txt`).
