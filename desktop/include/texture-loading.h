#pragma once

#include "Font.h"
#include <stdint.h>

// Custom texture bindings
void Texture_Load(const char *path, void *ptr, void (*callback)(void *ptr, int32_t width, int32_t height, void *data));
void Texture_Free(void *data);
void TextTexture_Render(const char *text, Font *font, uint32_t size, uint32_t color, void *ptr,
                        void (*callback)(void *ptr, int32_t width, int32_t height, void *data));
void TextTexture_Free(void *data);
