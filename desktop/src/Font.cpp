#include "Font.h"
#include "Log.h"
#include "utils.h"
#include <math.h>
#include <stdlib.h>

Font *Font::loadFromFile(const char *path) {
    Font *font = new Font(path);
    font->fontData = fileRead(path);
    if (stbtt_InitFont(&font->fontInfo, font->fontData, stbtt_GetFontOffsetForIndex(font->fontData, 0)) == 0) {
        Log::error("Can't load font file: %s", path);
    }
    font->loaded = true;
    return font;
}

int32_t Font::measureText(const char *text, uint32_t size) {
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, size);
    int xPadding = size / 4;
    int width = xPadding * 2;
    char *c = (char *)text;
    while (*c != '\0') {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&fontInfo, *c, &advance, &lsb);
        width += advance * scale;
        if (*(c + 1) != '\0') {
            width += stbtt_GetCodepointKernAdvance(&fontInfo, *c, *(c + 1)) * scale;
        }
        c++;
    }
    return width;
}

// Buggy (https://github.com/bplaat/plaatcraft/blob/main/src/font.c#L39)
void *Font::renderText(const char *text, uint32_t size, uint32_t color, int32_t *bitmapWidth, int32_t *bitmapHeight) {
    float scale = stbtt_ScaleForPixelHeight(&fontInfo, size);
    int xPadding = size / 4;

    int width = measureText(text, size);
    int height = size;
    uint8_t *bitmap = (uint8_t *)malloc(height * width);
    for (int i = 0; i < height * width; i++) {
        bitmap[i] = 0;
    }

    int x = xPadding;
    int ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &line_gap);
    ascent = round(ascent * scale);
    descent = round(descent * scale);

    char *c = (char *)text;
    while (*c != '\0') {
        int advance, lsb;
        stbtt_GetCodepointHMetrics(&fontInfo, *c, &advance, &lsb);

        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&fontInfo, *c, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        int y = ascent + c_y1;

        int character_width, character_height;
        uint8_t *character_bitmap =
            stbtt_GetCodepointBitmap(&fontInfo, 0, scale, *c, &character_width, &character_height, 0, 0);
        for (int32_t cy = 0; cy < character_height; cy++) {
            for (int32_t cx = 0; cx < character_width; cx++) {
                int32_t pos = (y + cy) * width + (x + cx);
                bitmap[pos] = MAX(bitmap[pos], character_bitmap[cy * character_width + cx]);
            }
        }
        stbtt_FreeBitmap(character_bitmap, NULL);

        x += advance * scale;
        if (*(c + 1)) {
            x += stbtt_GetCodepointKernAdvance(&fontInfo, *c, *(c + 1)) * scale;
        }
        c++;
    }

    uint8_t *coloredBitmap = (uint8_t *)malloc(height * width * 4);
    for (int32_t y = 0; y < height; y++) {
        for (int32_t x = 0; x < width; x++) {
            int32_t pos = (y * width + x) * 4;
            coloredBitmap[pos] = color & 0xff;
            coloredBitmap[pos + 1] = (color >> 8) & 0xff;
            coloredBitmap[pos + 2] = (color >> 16) & 0xff;
            coloredBitmap[pos + 3] = bitmap[y * width + x];
        }
    }
    *bitmapWidth = width;
    *bitmapHeight = height;
    return coloredBitmap;
}
