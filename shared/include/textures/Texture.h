#pragma once

#include <stdint.h>

class Texture {
  public:
    const char *path;
    int32_t width;
    int32_t height;
    bool transparent;
    bool pixelated;
    uint32_t texture;
    bool loaded;

#ifdef PLATFORM_WEB
    static void loadCallback(Texture *texture, int32_t width, int32_t height, void *data);
#endif

    static Texture *loadFromFile(const char *path, bool transparent, bool pixelated);

  protected:
    Texture(bool transparent, bool pixelated);

  public:
    void update(int32_t width, int32_t height, void *data);

    ~Texture();
};

// Custom texture bindings
#ifdef PLATFORM_WEB
extern "C" void Texture_Load(Texture *texture, const char *path,
                             void (*callback)(Texture *texture, int32_t width, int32_t height, void *data));
extern "C" void Texture_Free(void *data);
#endif
