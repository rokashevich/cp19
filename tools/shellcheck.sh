#!/bin/bash
# Детальная инфа по ошибкам: https://www.shellcheck.net/wiki
set -euo pipefail
cd `dirname $0`/..
set +o pipefail # shellcheck возвращает $?=1 в случае варнингов!
shellcheck -a -s bash -S style tools/*.sh|tee >(grep -cE ' SC[0-9]{4}: ')
