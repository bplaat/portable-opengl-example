#!/bin/sh

# Very experimental!!!

PATH=$PATH:$ANDROID_HOME/build-tools/31.0.0:$ANDROID_HOME/platform-tools:$ANDROID_NDK_ROOT/toolchains/llvm/prebuilt/darwin-x86_64/bin
PLATFORM=$ANDROID_HOME/platforms/android-31/android.jar

eval "$(grep "export " build.sh)"
password="android"
main_activity=".MainActivity"

if [ "$1" == "key" ]; then
    keytool -genkey -validity 7120 -keystore keystore.jks -keyalg RSA -keysize 4096 -storepass $password -keypass $password

elif [ "$1" == "log" ]; then
    adb logcat -c
    adb logcat *:E

else
    mkdir -p android/build

    echo "Compiling resources files"
    rm -rf android/build/res
    mkdir android/build/res
    if aapt2 compile --dir android/app/res -o android/build/res; then
        if aapt2 link android/build/res/*.flat --manifest android/app/AndroidManifest.xml -A assets \
            --java android/app/src -I $PLATFORM -o android/build/$app_name-unaligned.apk
        then
            echo ""
        else
            exit
        fi
    fi

    echo "Compiling java code"
    mkdir -p android/build/src
    find android/app/src -name *.java > android/build/sources.txt
    if javac -Xlint -cp $PLATFORM -d android/build/src @android/build/sources.txt; then
        echo ""
    else
        exit
    fi
    find android/build/src -name *.class > android/build/classes.txt
    if [ "$(uname -s)" == "Linux" ] || [ "$(uname -s)" == "Darwin" ]; then
        d8 --release --lib $PLATFORM --min-api 21 --output android/build @android/build/classes.txt
    else
        d8.bat --release --lib $PLATFORM --min-api 21 --output android/build @android/build/classes.txt
    fi
    cd android/build
    aapt add $app_name-unaligned.apk classes.dex
    cd ../..

    # Compile every c and cpp file if it is newer the the previous object file
    echo "Compiling cpp code"
    javac -h android/include -cp $PLATFORM android/app/src/com/example/portablegl/LibGame.java android/app/src/com/example/portablegl/Texture.java
    rm android/app/src/com/example/portablegl/LibGame.class android/app/src/com/example/portablegl/Texture.class
    mkdir -p android/build/android
    for file in $(find android/src shared/src -name *.c -o -name *.cpp); do
        name=${file%.*}
        ext=${file##*.}
        if [[ ${file::6} = "shared" ]]; then
            module="shared"
            object="android/build/$module/${name:11}.o"
            folder=$(dirname ${name:11})
        else
            module="android"
            object="android/build/$module/${name:12}.o"
            folder=$(dirname ${name:12})
        fi

        if [[ "$folder" != "." ]]; then
            mkdir -p "android/build/$module/$folder"
        fi

        if [[ $file -nt $object ]]; then
            if [[ $ext = "c" ]]; then
                compiler="aarch64-linux-android31-clang --std=c11"
            else
                compiler="aarch64-linux-android31-clang++ --std=c++11"
            fi
            if $compiler -c -Wall -Wextra -Wpedantic $cflags -DPLATFORM_ANDROID -Iandroid/include -Ishared/include -nostdlib -fPIC -fno-exceptions -fno-rtti -Ofast $file -o $object; then
                echo $file
            else
                exit
            fi
        fi
    done

    mkdir -p android/build/lib/arm64-v8a
    if aarch64-linux-android31-clang++ $(find android/build/android android/build/shared -name *.o) -nostdlib -fPIC -fno-exceptions -fno-rtti -lc -lm -lGLESv3 -llog -shared -o android/build/lib/arm64-v8a/libgame.so; then
        echo ""
    else
        exit
    fi
    cd android/build
    aapt add $app_name-unaligned.apk lib/arm64-v8a/libgame.so
    objdump -x lib/arm64-v8a/libgame.so > lib/arm64-v8a/dump
    cd ../..

    echo "Packing and signing application"


    #  x86_64-linux-android31-clang++ -fPIC -Ofast -Iinclude $(find src -name *.cpp) -nostdlib -lm -lGLESv3 -llog -shared -o lib/x86_64/libgame.so
    #   armv7a-linux-androideabi31-clang++ -fPIC -Ofast -Iinclude $(find src -name *.cpp) -nostdlib -lm -lGLESv3 -llog -shared -o lib/armeabi-v7a/libgame.so
    #  i686-linux-android31-clang++ -fPIC -Ofast -Iinclude $(find src -name *.cpp) -nostdlib -lm -lGLESv3 -llog -shared -o lib/x86/libgame.so
    # lib/x86_64/libgame.so lib/armeabi-v7a/libgame.so lib/x86/libgame.so

    zipalign -f -p 4 android/build/$app_name-unaligned.apk android/build/$app_name.apk

    if [ "$(uname -s)" == "Linux" ] || [ "$(uname -s)" == "Darwin" ]; then
        apksigner sign --v4-signing-enabled false --ks android/keystore.jks --ks-pass pass:$password --ks-pass pass:$password android/build/$app_name.apk
    else
        apksigner.bat sign --v4-signing-enabled false --ks android/keystore.jks --ks-pass pass:$password --ks-pass pass:$password android/build/$app_name.apk
    fi

    echo "Installing and opening application"
    adb install -r android/build/$app_name.apk
    adb shell am start -n $app_package/$main_activity
fi
