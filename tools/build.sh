#!/bin/bash
#
# Сборка проекта под десктоп или андроид.
#
# Запуск из корня проекта:
#   docker run --rm --mount type=bind,source=`pwd`,target=/w rokashevich/cp19-amd64-builder /w/tools/build.sh -t desktop
#   docker run --rm --mount type=bind,source=`pwd`,target=/w rokashevich/cp19-android-builder /w/tools/build.sh -t android
#
# Артефакты:
#   _desktop/cp19
#   SDL/build/github.rokashevich.cp19/app/build/outputs/apk/release/app-release-unsigned.apk


set -ueo pipefail

#######################################
# Print script usage info.
# Globals:
#   $0
# Arguments:
#   None
# Outputs:
#   STDOUT
# Returns:
#   None
#######################################
help()
{
  echo "Build project using docker images:"
  echo "  rokashevich/cp19-android-builder for android"
  echo "  rokashevich/cp19-amd64-builder for desktop"
  echo
  echo "Syntax: $0 [-h|t]"
  echo "options:"
  echo "h     Print this Help."
  echo "t     Architecture: desktop or android."
  echo
}

err() {
  echo "[$(date +'%Y-%m-%dT%H:%M:%S%z')]: $*" >&2
}

target="desktop" # по умолчанию собриаем под desktop

while getopts ":ht:" option; do
  case "${option}" in
    h) # display Help
      help
      exit 0
      ;;
    t) # target
      target="${OPTARG}";;
    \?) # Invalid option
      err "Error: Invalid option"
      exit
      ;;
  esac
done

if [[ "${target}" == "desktop" ]]; then
  cd "$(dirname "$0")/.."
  mkdir -p _desktop
  cd _desktop
  cmake ..
  make
elif [[ "${target}" == "android" ]]; then
  rm -rfv /w/SDL/build/github.rokashevich.cp19
  cd /w/SDL/build-scripts
  COPYSOURCE=0 ./androidbuild.sh github.rokashevich.cp19 /w/android/dummy.txt
  ln -s /w/src/*.cpp /w/SDL/build/github.rokashevich.cp19/app/jni/src
  ln -s /w/src/*.hpp /w/SDL/build/github.rokashevich.cp19/app/jni/src
  ln -s /w/generated/* /w/SDL/build/github.rokashevich.cp19/app/jni/src
  ln -s /w/glm/glm /w/SDL/build/github.rokashevich.cp19/app/jni/src/glm
  cp -rv /w/android/SDL/build/github.rokashevich.cp19/* /w/SDL/build/github.rokashevich.cp19
  cd /w/SDL/build/github.rokashevich.cp19
  ./gradlew assembleRelease
fi
