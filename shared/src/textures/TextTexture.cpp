#include "textures/TextTexture.h"
#include "texture-loading.h"

void TextTexture::renderCallback(void *ptr, int32_t width, int32_t height, void *data) {
    TextTexture *texture = (TextTexture *)ptr;
    texture->createTexture(width, height, data);
    TextTexture_Free(data);
}

TextTexture::TextTexture(const char *text, Font *font, uint32_t size, uint32_t color)
    : Texture(true, false), text(text), font(font), size(size), color(color) {
    TextTexture_Render(text, font, size, color, this, TextTexture::renderCallback);
}
