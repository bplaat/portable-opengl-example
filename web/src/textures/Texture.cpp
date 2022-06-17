#include "textures/Texture.h"
#include "Log.h"
#include <stddef.h>

void Texture::loadCallback(Texture *texture, int32_t width, int32_t height, void *data) {
    if (data == NULL) {
        Log::error("Can't load texture file: %s", texture->path);
    }
    texture->update(width, height, data);
    Texture_Free(data);
}

Texture *Texture::loadFromFile(const char *path, bool transparent, bool pixelated) {
    Texture *texture = new Texture(transparent, pixelated);
    texture->path = path;
    Texture_Load(texture, path, Texture::loadCallback);
    return texture;
}
