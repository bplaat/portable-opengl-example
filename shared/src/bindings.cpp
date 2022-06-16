#include "bindings.h"

#ifdef PLATFORM_DESKTOP

    #include "stb_image/stb_image.h"

void print(const char *string) {
    printf("%s", string);
}

void println(const char *string) {
    printf("%s\n", string);
}

void Texture_Load(const char *path, void *ptr, void (*callback)(void *ptr, int32_t width, int32_t height, void *data)) {
    int32_t width, height, channels;
    void *data = stbi_load(path, &width, &height, &channels, 0);
    callback(ptr, width, height, data);
}

void Texture_Free(void *data) {
    stbi_image_free(data);
}

#endif

#ifdef PLATFORM_WEB

uint32_t bumpPtr = (uint32_t)&__heap_base;

__attribute__((export_name("malloc"))) void *malloc(size_t size) {
    uint32_t ptr = bumpPtr;
    bumpPtr += size;
    bumpPtr += bumpPtr % 16; // Align all pointers to 16 bytes needed for SIMD
    return (void *)ptr;
}

__attribute__((export_name("free"))) void free(void *ptr) {
    // Nothing to do here :)
}

void *operator new(size_t size) {
    return malloc(size);
}

void operator delete(void *ptr) noexcept {
    free(ptr);
}

#endif
