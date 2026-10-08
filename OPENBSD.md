# RStudio Server on OpenBSD

This branch adds experimental support for building and running RStudio Server on OpenBSD.

## Tested configuration

The port has been tested successfully with:

- OpenBSD 7.9 amd64
- RStudio 2026.10.999-dev+999 "Blue Mistflower"
- R 4.5.2
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

## Notes

This is an experimental OpenBSD port and is not an officially supported Posit platform.

The current port includes OpenBSD-specific adaptations for process handling, resource detection, file monitoring, PAM include handling, Boost extraction, RapidJSON compatibility, Panmirror/esbuild, and related build-system differences.
