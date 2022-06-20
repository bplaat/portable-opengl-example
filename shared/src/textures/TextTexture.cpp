#include "textures/TextTexture.h"
#include "std.h"

TextTexture *TextTexture::fromText(const char *text, Font *font, uint32_t size, uint32_t color) {
    TextTexture *textTexture = new TextTexture(text, font, size, color);

    int32_t width, height;
    void *data = font->renderText(text, size, color, &width, &height);
    textTexture->update(width, height, data);

#if defined(PLATFORM_ANDROID) || defined(PLATFORM_DESKOP)
    free(data);
#endif
#ifdef PLATFORM_WEB
    Texture_Free(data);
#endif
    return textTexture;
}

TextTexture::TextTexture(const char *text, Font *font, uint32_t size, uint32_t color)
    : Texture(true, false), text(text), font(font), size(size), color(color) {}
