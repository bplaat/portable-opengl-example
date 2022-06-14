if [[ $1 == 'format' ]]; then
    clang-format -i $(find . -name *.h) $(find . -name *.c) $(find . -name *.cpp)
elif [[ $1 == 'desktop' ]]; then
    if g++ -DPLATFORM_DESKTOP -Ofast -Ishared/include $(find shared/src -name *.cpp) \
        -Idesktop/include $(find desktop/src -name *.c) $(find desktop/src -name *.cpp) \
        $(pkg-config --cflags --libs glfw3) -o desktop/game
    then
        ./desktop/game
    fi
else
    clang -DPLATFORM_WEB -Os -Ishared/include $(find shared/src -name *.cpp) --target=wasm32 -nostdlib  \
        -Wl,--no-entry -Wl,--allow-undefined -Wl,-z,stack-size=$[256 * 1024] -o web/game.wasm
fi
