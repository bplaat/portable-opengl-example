#include "texture-loading.h"
#include "Font.h"
#include "stb_image/stb_image.h"

void Texture_Load(const char *path, void *ptr, void (*callback)(void *ptr, int32_t width, int32_t height, void *data)) {
    int32_t width, height, channels;
    void *data = stbi_load(path, &width, &height, &channels, 0);
    callback(ptr, width, height, data);
}

void Texture_Free(void *data) {
    stbi_image_free(data);
}

void TextTexture_Render(const char *text, Font *font, uint32_t size, uint32_t color, void *ptr,
                        void (*callback)(void *ptr, int32_t width, int32_t height, void *data)) {
    int32_t width, height;
    void *data = font->renderText(text, size, color, &width, &height);
    callback(ptr, width, height, data);
}

void TextTexture_Free(void *data) {
    free(data);
}
