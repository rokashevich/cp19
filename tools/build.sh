#!/bin/bash
# docker run --rm --mount type=bind,source=`pwd`,target=/w rokashevich/cp19-amd64-builder /w/tools/build.sh
set -euo pipefail
cd `dirname $0`/..
mkdir -p b
cd b
cmake ..
make $*
