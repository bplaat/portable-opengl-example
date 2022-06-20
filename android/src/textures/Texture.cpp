#include "textures/Texture.h"
#include "Log.h"
#include "std.h"

Texture *Texture::loadFromFile(const char *path, bool transparent, bool pixelated) {
    Texture *texture = new Texture(transparent, pixelated);
    texture->path = path;

    // Call Texture_Load method that loads and decodes a bitmap from the assets folder
    jmethodID Texture_LoadId =
        env->GetStaticMethodID(libGameClass, "Texture_Load", "(Ljava/lang/String;)Lcom/example/portablegl/Texture;");
    jstring pathString = env->NewStringUTF(path);
    jobject textureObject = env->CallStaticObjectMethod(libGameClass, Texture_LoadId, pathString);
    env->DeleteLocalRef(pathString);
    if (textureObject == NULL) {
        Log::error("Can't load texture file: %s", texture->path);
    }

    jclass textureClass = env->GetObjectClass(textureObject);
    jfieldID widthId = env->GetFieldID(textureClass, "width", "I");
    jfieldID heightId = env->GetFieldID(textureClass, "height", "I");
    jfieldID dataId = env->GetFieldID(textureClass, "data", "[I");

    jint width = env->GetIntField(textureObject, widthId);
    jint height = env->GetIntField(textureObject, heightId);
    jintArray dataArray = (jintArray)env->GetObjectField(textureObject, dataId);
    uint8_t *data = (uint8_t *)env->GetIntArrayElements(dataArray, NULL);

    // Convert weird Android color format to a standard RGB(A) one
    uint8_t *convertedData = (uint8_t *)malloc(width * height * (transparent ? 4 : 3));
    if (transparent) {
        for (int32_t y = 0; y < height; y++) {
            for (int32_t x = 0; x < width; x++) {
                uint8_t alpha = data[(y * width + x) * 4 + 3];
                uint8_t red = data[(y * width + x) * 4 + 2];
                uint8_t green = data[(y * width + x) * 4 + 1];
                uint8_t blue = data[(y * width + x) * 4];
                convertedData[(y * width + x) * 4] = red;
                convertedData[(y * width + x) * 4 + 1] = green;
                convertedData[(y * width + x) * 4 + 2] = blue;
                convertedData[(y * width + x) * 4 + 3] = alpha;
            }
        }
    } else {
        for (int32_t y = 0; y < height; y++) {
            for (int32_t x = 0; x < width; x++) {
                uint8_t red = data[(y * width + x) * 4 + 2];
                uint8_t green = data[(y * width + x) * 4 + 1];
                uint8_t blue = data[(y * width + x) * 4];
                convertedData[(y * width + x) * 3] = red;
                convertedData[(y * width + x) * 3 + 1] = green;
                convertedData[(y * width + x) * 3 + 2] = blue;
            }
        }
    }
    texture->update(width, height, convertedData);

    // No cleanup needed because all Java Objects are garbage collected I think
    free(convertedData);

    return texture;
}
