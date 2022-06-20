#!/bin/sh

eval "$(grep "export " build.sh)"
password="android"
main_activity=".MainActivity"

PATH=$PATH:$ANDROID_HOME/build-tools/31.0.0:$ANDROID_HOME/platform-tools:$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/$android_ndk_platform/bin
PLATFORM=$ANDROID_HOME/platforms/android-31/android.jar

if [ "$1" == "key" ]; then
    keytool -genkey -validity 7120 -keystore keystore.jks -keyalg RSA -keysize 4096 -storepass $password -keypass $password

elif [ "$1" == "log" ]; then
    adb logcat -c
    adb logcat PortableGL:D *:E SystemUpdate:S

else
    mkdir -p android/build

    echo "Compiling resources files"
    rm -rf android/build/res
    mkdir android/build/res
    if aapt2 compile --dir android/app/res -o android/build/res; then
        if aapt2 link android/build/res/*.flat --manifest android/app/AndroidManifest.xml -A assets \
            --java android/app/src -I $PLATFORM -o android/build/$app_name-unaligned.apk
        then
            echo "res/"
        else
            exit
        fi
    fi

    echo "Compiling java code"
    mkdir -p android/build/src
    find android/app/src -name *.java > android/build/sources.txt
    if javac -Xlint -cp $PLATFORM -d android/build/src @android/build/sources.txt; then
        echo "src/"
    else
        exit
    fi

    find android/build/src -name *.class > android/build/classes.txt
    if [ "$(uname -s)" == "Linux" ] || [ "$(uname -s)" == "Darwin" ]; then
        d8 --release --lib $PLATFORM --min-api 21 --output android/build @android/build/classes.txt
    else
        d8.bat --release --lib $PLATFORM --min-api 21 --output android/build @android/build/classes.txt
    fi
    echo "classes.dex"
    cd android/build
    aapt add $app_name-unaligned.apk classes.dex > /dev/null
    cd ../..

    # Compile the native code to a shared object to goes in to the apk lib/ folder
    echo "Compiling cpp code"
    javac -h android/include -cp $PLATFORM android/app/src/com/example/portablegl/LibGame.java android/app/src/com/example/portablegl/Texture.java
    rm android/app/src/com/example/portablegl/LibGame.class android/app/src/com/example/portablegl/Texture.class

    if [[ $1 = "release" ]]; then
        type="release"
        cflags=""
        archs="aarch64 armv7a x86_64 i686"
    else
        type="debug"
        cflags="-DDEBUG"
        archs=$android_debug_arch
    fi

    rm -rf android/build/lib
    for arch in $archs; do
        # Compile every c and cpp file if it is newer the the previous object file
        mkdir -p android/build/objects/$arch/$type/shared android/build/objects/$arch/$type/android
        for file in $(find shared/src android/src -name *.c -o -name *.cpp); do
            name=${file%.*}
            ext=${file##*.}
            if [[ ${file::6} = "shared" ]]; then
                module="shared"
                object="android/build/objects/$arch/$type/$module/${name:11}.o"
                folder=$(dirname ${name:11})
            else
                module="android"
                object="android/build/objects/$arch/$type/$module/${name:12}.o"
                folder=$(dirname ${name:12})
            fi
            if [[ "$folder" != "." ]]; then
                mkdir -p "android/build/objects/$arch/$type/$module/$folder"
            fi

            if [[ $file -nt $object ]]; then
                if [[ $ext = "c" ]]; then
                    if [[ $arch = "armv7a" ]]; then
                        compiler="armv7a-linux-androideabi31-clang --std=c11 -mfpu=vfpv3-d16"
                    else
                        compiler="$arch-linux-android31-clang --std=c11"
                    fi
                else
                    if [[ $arch = "armv7a" ]]; then
                        compiler="armv7a-linux-androideabi31-clang++ --std=c++11 -mfpu=vfpv3-d16"
                    else
                        compiler="$arch-linux-android31-clang++ --std=c++11"
                    fi
                fi
                if $compiler -c -Wall -Wextra -Wpedantic $cflags -DPLATFORM_ANDROID -Iandroid/include -Ishared/include -nostdlib -fPIC -fno-exceptions -fno-rtti -Ofast $file -o $object; then
                    echo $file
                else
                    exit
                fi
            fi
        done

        # Link final shared object
        if [[ $arch = "aarch64" ]]; then abi="arm64-v8a"; fi
        if [[ $arch = "armv7a" ]]; then abi="armeabi-v7a"; fi
        if [[ $arch = "x86_64" ]]; then abi="x86_64"; fi
        if [[ $arch = "i686" ]]; then abi="x86"; fi
        mkdir -p android/build/lib/$abi
        if [[ $arch = "armv7a" ]]; then
            compiler="armv7a-linux-androideabi31-clang++"
        else
            compiler="$arch-linux-android31-clang++"
        fi
        if $compiler $(find android/build/objects/$arch/$type -name *.o) -nostdlib -lc -lm -lGLESv3 -llog \
            -shared -o android/build/lib/$abi/libgame.so
        then
            echo "lib/$abi/libgame.so"
        else
            exit
        fi

        # Add it to the apk
        cd android/build
        aapt add $app_name-unaligned.apk lib/$abi/libgame.so > /dev/null
        cd ../..
    done

    echo "Aligning and signing application"
    zipalign -f -p 4 android/build/$app_name-unaligned.apk android/build/$app_name.apk
    if [ "$(uname -s)" == "Linux" ] || [ "$(uname -s)" == "Darwin" ]; then
        apksigner sign --v4-signing-enabled false --ks android/keystore.jks --ks-pass pass:$password --ks-pass pass:$password android/build/$app_name.apk
    else
        apksigner.bat sign --v4-signing-enabled false --ks android/keystore.jks --ks-pass pass:$password --ks-pass pass:$password android/build/$app_name.apk
    fi

    if [[ $1 != "release" ]]; then
        echo "Installing and opening application"
        adb install -r android/build/$app_name.apk
        adb shell am start -n $app_package/$main_activity
    fi
fi
