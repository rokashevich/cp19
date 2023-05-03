#!/bin/bash
set -euo pipefail
cd `dirname $0`/..
shellcheck tools/*.sh
