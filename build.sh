# Remove build folders
if [[ $1 == "clean" ]]; then
    rm -rf desktop/build web/build

# Format source code
elif [[ $1 == "format" ]]; then
    clang-format -i $(find . -name *.h) $(find . -name *.c) $(find . -name *.cpp)

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

    if g++ $(find desktop/build -name *.o) $(pkg-config --libs glfw3) -o desktop/build/game; then
        ./desktop/build/game
    fi

# Web build script
else
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
fi
