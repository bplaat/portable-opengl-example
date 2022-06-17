#pragma once

#include "std.h"

#ifdef PLATFORM_DESKTOP
    #include "stb_truetype/stb_truetype.h"
#endif

class Font {
  public:
    const char *path;

#ifdef PLATFORM_DESKTOP
    uint8_t *fontData;
    stbtt_fontinfo fontInfo;
#endif

    Font(const char *path);

#ifdef PLATFORM_DESKTOP
    int32_t measureText(const char *text, uint32_t size);

    uint8_t *renderText(const char *text, uint32_t size, uint32_t color, int32_t *bitmapWidth, int32_t *bitmapHeight);
#endif
};
