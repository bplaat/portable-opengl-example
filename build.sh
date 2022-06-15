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
if [[ $1 == "clean" ]]; then
    rm -rf desktop/build web/build

# Format source code
elif [[ $1 == "format" ]]; then
    clang-format -i $(find . -name *.h -o -name *.c -o -name *.cpp)

# Desktop build script
elif [[ $1 == "desktop" ]]; then
    mkdir -p desktop/build/desktop desktop/build/shared

    for file in $(find shared/src -name *.cpp); do
        name=${file%.*}
        object="desktop/build/shared/${name:11}.o"

        folder=$(dirname ${name:11})
        if [[ "$folder" != "." ]]; then
            mkdir -p "desktop/build/shared/$folder"
        fi

        if [[ $file -nt $object ]]; then
            if g++ -DPLATFORM_DESKTOP -Ofast -c -Ishared/include -Idesktop/include $file -o $object $(pkg-config --cflags glfw3); then
                echo $file
            else
                exit
            fi
        fi
    done

    for file in $(find desktop/src -name *.c -o -name *.cpp); do
        name=${file%.*}
        ext=${file##*.}
        object="desktop/build/desktop/${name:11}.o"

        folder=$(dirname ${name:11})
        if [[ "$folder" != "." ]]; then
            mkdir -p "desktop/build/desktop/$folder"
        fi

        if [[ $file -nt $object ]]; then
            if [[ $ext  == "c" ]]; then compiler="gcc"; else compiler="g++"; fi
            if $compiler -DPLATFORM_DESKTOP -Ofast -c -Ishared/include -Idesktop/include $file -o $object $(pkg-config --cflags glfw3); then
                echo $file
            else
                exit
            fi
        fi
    done

    # Copy assets folder
    rm -rf desktop/build/assets
    cp -r assets desktop/build

    if g++ $(find desktop/build -name *.o) $(pkg-config --libs glfw3) -o desktop/build/game; then
        cd desktop/build
        ./game
    fi

# Web build script
else
    # Also build without WASM SIMD version
    if [[ $1 == "release" ]]; then
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
                if clang -DPLATFORM_WEB -Os -c -Ishared/include $file --target=wasm32 -o $object; then
                    echo $file
                else
                    exit
                fi
            fi
        done

        clang -Os $(find web/build -name *.o) --target=wasm32 -nostdlib -Wl,--no-entry \
            -Wl,--allow-undefined -Wl,-z,stack-size=$[256 * 1024] -o web/build/game.wasm

        wasm2wat web/build/game.wasm > web/build/game.wat
        sed -i "" "s/(export \"memory\" (memory 0))/(export \"memory\" (memory 0))\n  (export \"table\" (table 0))/" web/build/game.wat
        wat2wasm web/build/game.wat -o web/build/game.wasm

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
            if clang -DPLATFORM_WEB -Os -c -Ishared/include $file --target=wasm32 -msimd128 -o $object; then
                echo $file
            else
                exit
            fi
        fi
    done

    # Copy assets folder
    rm -rf web/build/assets
    cp -r assets web/build

    clang -Os $(find web/build -name *.o) --target=wasm32 -msimd128 -nostdlib -Wl,--no-entry \
        -Wl,--allow-undefined -Wl,-z,stack-size=$[256 * 1024] -o web/build/game-simd.wasm

    wasm2wat web/build/game-simd.wasm > web/build/game-simd.wat
    sed -i "" "s/(export \"memory\" (memory 0))/(export \"memory\" (memory 0))\n  (export \"table\" (table 0))/" web/build/game-simd.wat
    wat2wasm web/build/game-simd.wat -o web/build/game-simd.wasm
fi
