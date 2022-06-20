#pragma once

#include <stdint.h>
#ifdef PLATFORM_ANDROID
    #include <jni.h>
#endif
#ifdef PLATFORM_DESKTOP
    #include "stb_truetype/stb_truetype.h"
#endif

class Font {
  public:
    const char *path;
#ifdef PLATFORM_ANDROID
    jobject typeface;
#endif
#ifdef PLATFORM_DESKTOP
    uint8_t *fontData;
    stbtt_fontinfo fontInfo;
#endif
#ifdef PLATFORM_WEB
    uint32_t fontFace;
#endif
    bool loaded;

#ifdef PLATFORM_WEB
    static void loadCallback(Font *font, uint32_t fontFace);
#endif

    static Font *loadFromFile(const char *path);

    ~Font();

    int32_t measureText(const char *text, uint32_t size);

    void *renderText(const char *text, uint32_t size, uint32_t color, int32_t *bitmapWidth, int32_t *bitmapHeight);

  private:
    Font(const char *path);
};

// Custom font bindings
#ifdef PLATFORM_WEB
extern "C" void Font_Load(Font *font, const char *path, void (*callback)(Font *font, uint32_t fontFace));
extern "C" int32_t Font_MeasureText(uint32_t fontFace, const char *text, uint32_t size);
extern "C" void *Font_RenderText(uint32_t fontFace, const char *text, uint32_t size, uint32_t color,
                                 int32_t *bitmapWidth, int32_t *bitmapHeight);
extern "C" void Font_Free(uint32_t fontFace);
#endif
