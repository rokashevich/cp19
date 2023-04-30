#!/bin/bash
set -euo pipefail
cd `basename $0`
mkdir b
cd b
cmake ..
make
