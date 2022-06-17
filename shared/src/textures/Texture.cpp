#include "textures/Texture.h"
#include "opengl.h"

Texture::Texture(bool transparent, bool pixelated) : transparent(transparent), pixelated(pixelated), texture(0) {}

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
