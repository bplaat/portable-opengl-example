#include "textures/Texture.h"
#include "bindings.h"

void Texture::loadCallback(void *ptr, int32_t width, int32_t height, void *data) {
    Texture *texture = (Texture *)ptr;
    texture->width = width;
    texture->height = height;

    if (data == NULL) {
        print("Can't load image: ");
        println(texture->path);
    }

    glGenTextures(1, &texture->texture);
    glBindTexture(GL_TEXTURE_2D, texture->texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    texture->pixelated ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->pixelated ? GL_NEAREST : GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, texture->transparent ? GL_RGBA : GL_RGB, width, height, 0,
                 texture->transparent ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    freeTexture(data);
}

Texture::Texture(const char *path, bool transparent, bool pixelated)
    : path(path), transparent(transparent), pixelated(pixelated) {
    loadTexture(path, this, Texture::loadCallback);
}

Texture::~Texture() {
    glDeleteTextures(1, &texture);
}
