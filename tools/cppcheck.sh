#!/bin/bash
set -euo pipefail
cd `dirname $0`/..
cppcheck --enable=all --inconclusive -v src
