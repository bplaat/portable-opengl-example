#include "textures/Texture.h"
#include "Log.h"
#include "stb_image/stb_image.h"

Texture *Texture::loadFromFile(const char *path, bool transparent, bool pixelated) {
    Texture *texture = new Texture(transparent, pixelated);
    texture->path = path;

    int32_t width, height, channels;
    void *data = stbi_load(path, &width, &height, &channels, 0);
    if (data == NULL) {
        Log::error("Can't load texture file: %s", texture->path);
    }
    texture->create(width, height, data);
    stbi_image_free(data);
    return texture;
}
