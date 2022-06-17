#ifndef TEXT_TEXTURE_H
#define TEXT_TEXTURE_H

#include "Font.h"
#include "textures/Texture.h"

class TextTexture : public Texture {
  public:
    const char *text;
    Font *font;
    uint32_t size;
    uint32_t color;

    static void renderCallback(void *ptr, int32_t width, int32_t height, void *data);

    TextTexture(const char *text, Font *font, uint32_t size, uint32_t color);
};

#endif
