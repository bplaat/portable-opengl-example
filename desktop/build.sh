#!/bin/sh
eval "$(grep "export " build.sh)"
if [[ $1 = "release" ]]; then
    type="release"
    cflags=""
else
    type="debug"
    cflags="-DDEBUG"
fi

# Compile every c and cpp file if it is newer the the previous object file
mkdir -p desktop/build/objects/$type/shared desktop/build/objects/$type/desktop
for file in $(find shared/src desktop/src -name *.c -o -name *.cpp); do
    name=${file%.*}
    ext=${file##*.}
    if [[ ${file::6} = "shared" ]]; then
        module="shared"
        object="desktop/build/objects/$type/$module/${name:11}.o"
        folder=$(dirname ${name:11})
    else
        module="desktop"
        object="desktop/build/objects/$type/$module/${name:12}.o"
        folder=$(dirname ${name:12})
    fi
    if [[ "$folder" != "." ]]; then
        mkdir -p "desktop/build/objects/$type/$module/$folder"
    fi

    if [[ $file -nt $object ]]; then
        if [[ $ext = "c" ]]; then compiler="gcc --std=c11"; else compiler="g++ --std=c++11"; fi
        if $compiler -c -Wall -Wextra -Wpedantic $cflags -DPLATFORM_DESKTOP -Ishared/include -Idesktop/include \
            -Ofast $file -o $object $(pkg-config --cflags glfw3)
        then
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
if g++ $(find desktop/build/objects/$type -name *.o) $(pkg-config --libs glfw3) -o desktop/build/$app_name; then
    if [[ $type == "debug" ]]; then
        cd desktop/build
        ./$app_name
    fi
fi
