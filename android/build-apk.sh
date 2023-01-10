#!/bin/bash

[ -z $ANDROID ] && { echo "Set ANDROID environment vairable!"; exit; }

PATH="$ANDROID/ndk-bundle:$PATH"                # for 'ndk-build'
PATH="$ANDROID/ndk-bundle/simpleperf/bin:$PATH" # for 'android'

# ./sdkmanager --sdk_root=$ANDROID/sdk
# openjdk version "1.8.0_242" из Ubuntu
PATH="$ANDROID/platform-tools:$ANDROID/tools:$PATH" # for 'adb'
export ANDROID_HOME="$ANDROID"          # for gradle
export ANDROID_NDK_HOME="$ANDROID/android-ndk-r25b" #1e" # for gradle

PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"
rm -rf $PROJECT_ROOT/SDL/build/github.rokashevich.cp19

cd $PROJECT_ROOT/SDL/build-scripts
COPYSOURCE=0 ./androidbuild.sh github.rokashevich.cp19 $PROJECT_ROOT/src/main.cpp $PROJECT_ROOT/src/world.cpp

cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19/app/jni/src
rm *.cpp
ln -s $PROJECT_ROOT/src/* .
ln -s $PROJECT_ROOT/generated/* .
cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19/app/jni
ln -s $PROJECT_ROOT/glm .
cp -rv $PROJECT_ROOT/android/SDL/* $PROJECT_ROOT/SDL
cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19
./gradlew assembleDebug

cp -v app/build/outputs/apk/debug/app-debug.apk $PROJECT_ROOT/android

