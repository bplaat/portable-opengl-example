#include "textures/AssetTexture.h"

void AssetTexture::loadCallback(void *ptr, int32_t width, int32_t height, void *data) {
    AssetTexture *texture = (AssetTexture *)ptr;
    if (data == NULL) {
        print("Can't load image: ");
        println(texture->path);
    }
    texture->createTexture(width, height, data);
    Texture_Free(data);
}

AssetTexture::AssetTexture(const char *path, bool transparent, bool pixelated)
    : Texture(transparent, pixelated), path(path) {
    Texture_Load(path, this, AssetTexture::loadCallback);
}
