#include "Font.h"
#include "std.h"

Font *Font::loadFromFile(const char *path) {
    Font *font = new Font(path);
    jmethodID Font_LoadId =
        env->GetStaticMethodID(libGameClass, "Font_Load", "(Ljava/lang/String;)Landroid/graphics/Typeface;");
    font->typeface = env->CallStaticObjectMethod(libGameClass, Font_LoadId, env->NewStringUTF(path));
    font->loaded = true;
    return font;
}

int32_t Font::measureText(const char *text, uint32_t size) {
    jmethodID Font_MeasureTextId =
        env->GetStaticMethodID(libGameClass, "Font_MeasureText", "(Landroid/graphics/Typeface;Ljava/lang/String;I)I");
    return env->CallStaticIntMethod(libGameClass, Font_MeasureTextId, typeface, env->NewStringUTF(text), size);
}

void *Font::renderText(const char *text, uint32_t size, uint32_t color, int32_t *bitmapWidth, int32_t *bitmapHeight) {
    jmethodID Font_RenderTextId =
        env->GetStaticMethodID(libGameClass, "Font_RenderText",
                               "(Landroid/graphics/Typeface;Ljava/lang/String;II)Lcom/example/portablegl/Texture;");
    jobject textureObject =
        env->CallStaticObjectMethod(libGameClass, Font_RenderTextId, typeface, env->NewStringUTF(text), size, color);

    jclass textureClass = env->GetObjectClass(textureObject);
    jfieldID widthId = env->GetFieldID(textureClass, "width", "I");
    jfieldID heightId = env->GetFieldID(textureClass, "height", "I");
    jfieldID dataId = env->GetFieldID(textureClass, "data", "[I");

    jint width = env->GetIntField(textureObject, widthId);
    jint height = env->GetIntField(textureObject, heightId);
    jintArray dataArray = (jintArray)env->GetObjectField(textureObject, dataId);
    uint8_t *data = (uint8_t *)env->GetIntArrayElements(dataArray, NULL);

    // Convert weird Android color format to a standard RGBA one
    uint8_t *convertedData = (uint8_t *)malloc(width * height * 4);
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

    *bitmapWidth = width;
    *bitmapHeight = height;
    return convertedData;
}
