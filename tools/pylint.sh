#!/bin/bash
set -euo pipefail
cd `dirname $0`/..
pylint tools/*.py
