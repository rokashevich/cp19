#!/bin/bash

PATH="$ANDROID/ndk:$PATH"                   # for 'ndk-build'
PATH="$ANDROID/tools:$PATH"             # for 'android'
PATH="$ANDROID/platform-tools:$PATH"    # for 'adb'
export ANDROID_HOME="$ANDROID"          # for gradle
export ANDROID_NDK_HOME="$ANDROID/ndk"      # for gradle

PROJECT_ROOT="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"

cd $PROJECT_ROOT/SDL/build-scripts
rm -rf $PROJECT_ROOT/SDL/build/github.rokashevich.cp19
./androidbuild.sh github.rokashevich.cp19 $PROJECT_ROOT/src/main.cpp

cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19/app/jni/src
rm main.cpp
ln -s $PROJECT_ROOT/src/*.*pp .

cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19/app/jni
ln -s $PROJECT_ROOT/glm .

cp -rv $PROJECT_ROOT/android/SDL/* $PROJECT_ROOT/SDL
cd $PROJECT_ROOT/SDL/build/github.rokashevich.cp19
./gradlew assembleDebug

