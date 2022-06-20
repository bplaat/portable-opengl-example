#!/bin/sh
eval "$(grep "export " build.sh)"
if [[ $1 = "release" ]]; then
    type="release"
    cflags=""
else
    type="debug"
    cflags="-DDEBUG"
fi

if [[ $1 = "release" ]]; then
    # Build without WASM SIMD version
    mkdir -p web/build/objects/$type/shared web/build/objects/$type/web
    for file in $(find shared/src web/src -name *.c -o -name *.cpp); do
        name=${file%.*}
        ext=${file##*.}
        if [[ ${file::3} = "web" ]]; then
            module="web"
            object="web/build/objects/$type/$module/${name:8}.o"
            folder=$(dirname ${name:8})
        else
            module="shared"
            object="web/build/objects/$type/$module/${name:11}.o"
            folder=$(dirname ${name:11})
        fi
        if [[ "$folder" != "." ]]; then
            mkdir -p "web/build/objects/$type/$module/$folder"
        fi

        if [[ $file -nt $object ]]; then
            if [[ $ext = "c" ]]; then compiler="clang --std=c11"; else compiler="clang++ --std=c++11"; fi
            if $compiler -c -Wall -Wextra -Wpedantic $cflags -DPLATFORM_WEB -Ishared/include -Iweb/include \
                -Os --target=wasm32 $file -o $object
            then
                echo $file
            else
                exit
            fi
        fi
    done

    # Link final wasm bundle
    wasm-ld $(find web/build/objects/$type -name *.o) --no-entry --allow-undefined \
        -z,stack-size=$[256 * 1024] --export-table -o web/build/$app_name.wasm
fi

# Build WASM SIMD version
mkdir -p web/build/objects/simd/$type/shared web/build/objects/simd/$type/web
for file in $(find shared/src web/src -name *.c -o -name *.cpp); do
    name=${file%.*}
    ext=${file##*.}
    if [[ ${file::3} = "web" ]]; then
        module="web"
        object="web/build/objects/simd/$type/$module/${name:8}.o"
        folder=$(dirname ${name:8})
    else
        module="shared"
        object="web/build/objects/simd/$type/$module/${name:11}.o"
        folder=$(dirname ${name:11})
    fi
    if [[ "$folder" != "." ]]; then
        mkdir -p "web/build/objects/simd/$type/$module/$folder"
    fi

    if [[ $file -nt $object ]]; then
        if [[ $ext = "c" ]]; then compiler="clang --std=c11"; else compiler="clang++ --std=c++11"; fi
        if $compiler -c -Wall -Wextra -Wpedantic $cflags -DPLATFORM_WEB -D__WASM_SIMD__ -Ishared/include -Iweb/include \
            -Os --target=wasm32 -msimd128 $file -o $object
        then
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
wasm-ld $(find web/build/objects/simd/$type -name *.o) --no-entry --allow-undefined \
    -z,stack-size=$[256 * 1024] --export-table -o web/build/$app_name-simd.wasm
