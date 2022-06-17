#!/bin/sh
# A simple incremental cpp project build script
# ./build.sh ; Build the web wasm simd bundle for Firefox and Chrome (not Safari for the moment!)
# ./build.sh release ; Build the web wasm without simd bundle for all modern browsers
# ./build.sh desktop ; Build the desktop application and run it
# ./build.sh clean ; Remove all build folders (and so all the incremental builded objects)
# ./build.sh format ; Run the clang formatter over the whole c and cpp code base
# Tip: If you run a webserver with live refresh you wont have to manually refresh the webbrowser
# https://marketplace.visualstudio.com/items?itemName=ritwickdey.LiveServer

# Remove build folders
if [[ $1 = "clean" ]]; then
    rm -rf desktop/build web/build

# Format source code
elif [[ $1 = "format" ]]; then
    clang-format -i $(find . -name *.h -o -name *.c -o -name *.cpp)

# Desktop build script
elif [[ $1 = "desktop" ]]; then
    mkdir -p desktop/build/desktop desktop/build/shared

    for file in $(find shared/src desktop/src -name *.c -o -name *.cpp); do
        name=${file%.*}
        ext=${file##*.}
        if [[ ${file::6} = "shared" ]]; then module="shared"; else module="desktop"; fi
        object="desktop/build/$module/${name:11}.o"
        folder=$(dirname ${name:11})
        if [[ "$folder" != "." ]]; then
            mkdir -p "desktop/build/$module/$folder"
        fi

        if [[ $file -nt $object ]]; then
            if [[ $ext = "c" ]]; then compiler="gcc"; else compiler="g++"; fi
            if $compiler -c -DPLATFORM_DESKTOP -Ishared/include -Idesktop/include -Ofast $file -o $object $(pkg-config --cflags glfw3); then
                echo $file
            else
                exit
            fi
        fi
    done

    # Copy assets folder
    rm -rf desktop/build/assets
    cp -r assets desktop/build

    # Link final executable
    if g++ $(find desktop/build -name *.o) $(pkg-config --libs glfw3) -o desktop/build/game; then
        cd desktop/build
        ./game
    fi

# Web build script
else
    # Also build without WASM SIMD version
    if [[ $1 = "release" ]]; then
        rm -rf web/build/shared
        mkdir -p web/build/shared

        for file in $(find shared/src -name *.cpp); do
            name=${file%.*}
            object="web/build/shared/${name:11}.o"
            folder=$(dirname ${name:11})
            if [[ "$folder" != "." ]]; then
                mkdir -p "web/build/shared/$folder"
            fi

            if [[ $file -nt $object ]]; then
                if clang -c -DPLATFORM_WEB -Ishared/include -Os --target=wasm32 $file -o $object; then
                    echo $file
                else
                    exit
                fi
            fi
        done

        # Link final wasm bundle
        wasm-ld $(find web/build -name *.o) --no-entry --allow-undefined \
            -z,stack-size=$[256 * 1024] --export-table -o web/build/game.wasm

        rm -rf web/build/shared
    fi

    mkdir -p web/build/shared

    # Build WASM SIMD version
    for file in $(find shared/src -name *.cpp); do
        name=${file%.*}
        object="web/build/shared/${name:11}.o"
        folder=$(dirname ${name:11})
        if [[ "$folder" != "." ]]; then
            mkdir -p "web/build/shared/$folder"
        fi

        if [[ $file -nt $object ]]; then
            if clang -c -DPLATFORM_WEB -Ishared/include -Os --target=wasm32 -msimd128 $file -o $object; then
                echo $file
            else
                exit
            fi
        fi
    done

    # Copy assets folder
    rm -rf web/build/assets
    cp -r assets web/build

    # Link final wasm bundle
    wasm-ld $(find web/build -name *.o) --no-entry --allow-undefined \
        -z,stack-size=$[256 * 1024] --export-table -o web/build/game-simd.wasm
fi
