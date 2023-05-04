#!/bin/bash
set -euo pipefail
cd `dirname $0`/..
cppcheck --enable=all --inconclusive -v src 2>&1 1>/dev/null |tee >(grep -E ' \[[A-Za-z0-9]+\]$'|wc -l)
