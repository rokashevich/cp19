#!/bin/bash

[ -z $ANDROID ] && { echo "Set ANDROID environment vairable!"; exit; }

PATH="$ANDROID/ndk:$PATH"                # for 'ndk-build'
PATH="$ANDROID/ndk/simpleperf/bin:$PATH" # for 'android'

# ./sdkmanager --sdk_root=$ANDROID/sdk
# openjdk version "1.8.0_242" из Ubuntu
PATH="$ANDROID/sdk/platform-tools:$PATH"    # for 'adb'
export ANDROID_HOME="$ANDROID/sdk"          # for gradle
export ANDROID_NDK_HOME="$ANDROID/ndk"      # for gradle

PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"
rm -rf $PROJECT_ROOT/SDL/build/github.rokashevich.cp19

cd $PROJECT_ROOT/SDL/build-scripts
read -p "android build"
COPYSOURCE=0 ./androidbuild.sh github.rokashevich.cp19 $PROJECT_ROOT/src/main.cpp $PROJECT_ROOT/src/gameworld.cpp

cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19/app/jni/src
rm *.cpp
ln -s $PROJECT_ROOT/src/* .
cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19/app/jni
ln -s $PROJECT_ROOT/glm .
read -p "dbg"
cp -rv $PROJECT_ROOT/android/SDL/* $PROJECT_ROOT/SDL
cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19
read -p "gradlew"
./gradlew assembleDebug

