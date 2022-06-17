#include "textures/Texture.h"
#include "opengl.h"
#include "std.h"
#include "texture-loading.h"

void Texture::loadCallback(void *ptr, int32_t width, int32_t height, void *data) {
    Texture *texture = (Texture *)ptr;
    if (data == NULL) {
        printf("[ERROR] Can't load texture file: %s\n", texture->path);
    }
    texture->createTexture(width, height, data);
    Texture_Free(data);
}

Texture *Texture::fromFile(const char *path, bool transparent, bool pixelated) {
    Texture *texture = new Texture(transparent, pixelated);
    texture->path = path;
    Texture_Load(path, texture, Texture::loadCallback);
    return texture;
}

Texture::Texture(bool transparent, bool pixelated)
    : path(NULL), transparent(transparent), pixelated(pixelated), texture(0) {}

void Texture::createTexture(int32_t width, int32_t height, void *data) {
    this->width = width;
    this->height = height;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    pixelated ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelated ? GL_NEAREST : GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, transparent ? GL_RGBA : GL_RGB, width, height, 0, transparent ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
    if (texture != 0) {
        glDeleteTextures(1, &texture);
    }
}
