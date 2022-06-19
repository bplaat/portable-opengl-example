#!/bin/sh
# A simple incremental build script for android, desktop and web
# ./build.sh clean ; Remove all build folders (and so all the incremental builded objects)
# ./build.sh format ; Run the clang formatter over the whole c and cpp code base
# ./build.sh android ; Build and run a debug version of the Android application (only ARM64)
# ./build.sh android release ; Build a release version of the Android app (all instructionsets)
# ./build.sh desktop ; Build and run a debug version of the desktop app
# ./build.sh desktop release ; Build a release version of the desktop app
# ./build.sh web ; Build a debug version of the WASM bundle (only with SIMD so only for Firefox and Chrome)
# ./build.sh web release ; Build a release version of the WASM bundle (for all browsers)
# Tip for web: If you run a webserver with live refresh you wont have to manually refresh the webbrowser
# https://marketplace.visualstudio.com/items?itemName=ritwickdey.LiveServer

export app_name="portablegl"
export app_package="com.example.portablegl"
export app_version="0.1.0"

if [[ $1 = "clean" ]]; then
    rm -rf android/build desktop/build web/build

elif [[ $1 = "format" ]]; then
    clang-format -i $(find . -name *.h -o -name *.c -o -name *.cpp)

elif [[ $1 = "android" ]]; then
    if [[ $2 = "key" ]]; then
        ./android/build.sh key
    elif [[ $2 = "log" ]]; then
        ./android/build.sh log
    elif [[ $2 = "release" ]]; then
        ./android/build.sh release
    else
        ./android/build.sh
    fi

elif [[ $1 = "desktop" ]]; then
    if [[ $2 = "release" ]]; then
        ./desktop/build.sh release
    else
        ./desktop/build.sh
    fi

elif [[ $1 = "web" ]]; then
    if [[ $2 = "release" ]]; then
        ./web/build.sh release
    else
        ./web/build.sh
    fi

else
    echo "Use one of the subcommands to do something..."
fi
