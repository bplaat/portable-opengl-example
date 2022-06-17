#pragma once

#include <stdbool.h>
#include <stdint.h>

class Texture {
  public:
    const char *path;
    int32_t width;
    int32_t height;
    bool transparent;
    bool pixelated;
    uint32_t texture;

    static void loadCallback(void *ptr, int32_t width, int32_t height, void *data);

    static Texture *fromFile(const char *path, bool transparent, bool pixelated);

    Texture(bool transparent, bool pixelated);

    void createTexture(int32_t width, int32_t height, void *data);

    ~Texture();
};
