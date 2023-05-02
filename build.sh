#!/bin/bash
set -euo pipefail
cd `dirname $0`
mkdir b
cd b
cmake ..
make
