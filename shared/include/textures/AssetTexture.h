#pragma once

#include "textures/Texture.h"

class AssetTexture : public Texture {
  public:
    const char *path;

    static void loadCallback(void *ptr, int32_t width, int32_t height, void *data);

    AssetTexture(const char *path, bool transparent, bool pixelated);
};
