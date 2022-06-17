#pragma once

#include <stdbool.h>
#include <stdint.h>

class Texture {
  public:
    int32_t width;
    int32_t height;
    bool transparent;
    bool pixelated;
    uint32_t texture;

    Texture(bool transparent, bool pixelated);

    void createTexture(int32_t width, int32_t height, void *data);

    ~Texture();
};
