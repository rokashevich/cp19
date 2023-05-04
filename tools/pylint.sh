#!/bin/bash
set -ue
cd `dirname $0`/..
pylint -v --exit-zero tools/*.py|tee >(grep -E ': [A-Z]+[0-9]+: '|wc -l)
