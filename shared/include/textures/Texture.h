#ifndef TEXTURE_H
#define TEXTURE_H

#include "bindings.h"

extern "C" {

#ifdef PLATFORM_WEB
__attribute__((export_name("textureLoadCallback")))
#endif
void textureLoadCallback(void *ptr, int32_t width, int32_t height, void *data);
}

class Texture {
  public:
    const char *path;
    int32_t width;
    int32_t height;
    bool transparent;
    bool pixelated;
    uint32_t texture;

    Texture(const char *path, bool transparent, bool pixelated);

    ~Texture();
};

#endif
