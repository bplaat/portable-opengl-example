#pragma once

#include "Font.h"
#include "textures/Texture.h"

class TextTexture : public Texture {
  public:
    const char *text;
    Font *font;
    uint32_t size;
    uint32_t color;

    static TextTexture *fromText(const char *text, Font *font, uint32_t size, uint32_t color);

  private:
    TextTexture(const char *text, Font *font, uint32_t size, uint32_t color);
};
