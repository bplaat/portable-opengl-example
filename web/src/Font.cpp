#include "Font.h"
#include "Log.h"

void Font::loadCallback(Font *font, uint32_t fontFace) {
    if (fontFace == 0) {
        Log::error("Can't load font file: %s", font->path);
    }
    font->fontFace = fontFace;
    font->loaded = true;
}

Font *Font::loadFromFile(const char *path) {
    Font *font = new Font(path);
    Font_Load(font, path, Font::loadCallback);
    return font;
}

int32_t Font::measureText(const char *text, uint32_t size) {
    return Font_MeasureText(fontFace, text, size);
}

void *Font::renderText(const char *text, uint32_t size, uint32_t color, int32_t *bitmapWidth, int32_t *bitmapHeight) {
    return Font_RenderText(fontFace, text, size, color, bitmapWidth, bitmapHeight);
}
