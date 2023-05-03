#!/bin/bash
set -euo pipefail
cd `dirname $0`
mkdir -p b
cd b
cmake ..
make $*
