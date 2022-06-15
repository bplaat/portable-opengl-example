#ifndef TEXTURE_H
#define TEXTURE_H

#include "bindings.h"

class Texture {
  public:
    const char *path;
    int32_t width;
    int32_t height;
    bool transparent;
    bool pixelated;
    uint32_t texture;

    static void loadCallback(void *ptr, int32_t width, int32_t height, void *data);

    Texture(const char *path, bool transparent, bool pixelated);

    ~Texture();
};

#endif
