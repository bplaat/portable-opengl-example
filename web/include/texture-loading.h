#pragma once

#include <stdint.h>

// Custom texture bindings
extern "C" {
extern void Texture_Load(const char *path, void *ptr,
                         void (*callback)(void *ptr, int32_t width, int32_t height, void *data));
extern void Texture_Free(void *data);
extern void TextTexture_Render(const char *text, void *font, uint32_t size, uint32_t color, void *ptr,
                               void (*callback)(void *ptr, int32_t width, int32_t height, void *data));
extern void TextTexture_Free(void *data);
}
