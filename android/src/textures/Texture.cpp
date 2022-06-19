#include "textures/Texture.h"
#include "Log.h"
#include "com_example_portablegl_LibGame.h"
#include "std.h"

Texture *Texture::loadFromFile(const char *path, bool transparent, bool pixelated) {
    Texture *texture = new Texture(transparent, pixelated);
    texture->path = path;

    jmethodID Texture_Load =
        env->GetStaticMethodID(libGameClass, "Texture_Load", "(Ljava/lang/String;)Lcom/example/portablegl/Texture;");
    jobject textureObject = env->CallStaticObjectMethod(libGameClass, Texture_Load, env->NewStringUTF(path));
    if (textureObject == NULL) {
        Log::error("Can't load texture file: %s", texture->path);
    }

    jclass textureClass = env->GetObjectClass(textureObject);
    jfieldID widthId = env->GetFieldID(textureClass, "width", "I");
    jint width = env->GetIntField(textureObject, widthId);
    jfieldID heightId = env->GetFieldID(textureClass, "height", "I");
    jint height = env->GetIntField(textureObject, heightId);
    jfieldID dataId = env->GetFieldID(textureClass, "data", "[I");
    jintArray dataArray = (jintArray)env->GetObjectField(textureObject, dataId);
    uint8_t *data = (uint8_t *)env->GetIntArrayElements(dataArray, NULL);

    // Hacky color conversion
    for (int32_t y = 0; y < height; y++) {
        for (int32_t x = 0; x < height; x++) {
            uint8_t alpha = data[(y * width + x) * 4 + 3];
            uint8_t red = data[(y * width + x) * 4 + 2];
            uint8_t green = data[(y * width + x) * 4 + 1];
            uint8_t blue = data[(y * width + x) * 4];
            data[(y * width + x) * 4] = red;
            data[(y * width + x) * 4 + 1] = green;
            data[(y * width + x) * 4 + 2] = blue;
            data[(y * width + x) * 4 + 3] = alpha;
        }
    }
    texture->update(width, height, data);

    // No cleanup you know :)

    return texture;
}
