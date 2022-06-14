clang -Iinclude $(find src -name *.cpp) --target=wasm32 -Os -flto -nostdlib \
    -Wl,--no-entry -Wl,--allow-undefined -Wl,--lto-O3 \
    -Wl,-z,stack-size=$[256 * 1024] -o game.wasm
