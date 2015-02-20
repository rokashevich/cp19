# Native/SDL starter kit

SDL template for developing portable native mobile application.

It bundles the official template for android and iOS from the [SDL project](https://www.libsdl.org/).

## Pre-requisites

- [Android NDK](https://developer.android.com/tools/sdk/ndk/index.html)
- [Xcode](https://developer.apple.com/xcode/)
- [SDL sources](https://www.libsdl.org/tmp/SDL-2.0.4-9304.zip)

## Usage

- Bootstrap a new project
```
git clone https://github.com/proppy/native-sdl-starter-kit.git my-project
cd my-project
ln -s /path/to/sdl/source SDL
```

- Android build and deploy
```
cd android
android update project -p .
ndk-build
ant debug
adb install -r bin/SDLActivity-debug.apk
```

- iOS build and deploy
```
cd ios
open nativesdlstarterkit.xcodeproj
```
