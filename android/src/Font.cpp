#include "Font.h"
#include "Log.h"
#include "std.h"

Font *Font::loadFromFile(const char *path) {
    Font *font = new Font(path);
    jmethodID Font_LoadId =
        env->GetStaticMethodID(libGameClass, "Font_Load", "(Ljava/lang/String;)Landroid/graphics/Typeface;");
    jstring pathString = env->NewStringUTF(path);
    font->typeface = env->NewGlobalRef(env->CallStaticObjectMethod(libGameClass, Font_LoadId, pathString));
    env->DeleteLocalRef(pathString);
    font->loaded = true;
    return font;
}

Font::~Font() {
    env->DeleteGlobalRef(typeface);
}

int32_t Font::measureText(const char *text, uint32_t size) {
    jmethodID Font_MeasureTextId =
        env->GetStaticMethodID(libGameClass, "Font_MeasureText", "(Landroid/graphics/Typeface;Ljava/lang/String;I)I");
    jstring textString = env->NewStringUTF(text);
    jint width = env->CallStaticIntMethod(libGameClass, Font_MeasureTextId, typeface, textString, size);
    env->DeleteLocalRef(textString);
    return width;
}

void *Font::renderText(const char *text, uint32_t size, uint32_t color, int32_t *bitmapWidth, int32_t *bitmapHeight) {
    jmethodID Font_RenderTextId =
        env->GetStaticMethodID(libGameClass, "Font_RenderText",
                               "(Landroid/graphics/Typeface;Ljava/lang/String;II)Lcom/example/portablegl/Texture;");
    jstring textString = env->NewStringUTF(text);
    jobject textureObject =
        env->CallStaticObjectMethod(libGameClass, Font_RenderTextId, typeface, textString, size, color);
    env->DeleteLocalRef(textString);

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

    // No cleanup needed because all Java Objects are garbage collected I think
    *bitmapWidth = width;
    *bitmapHeight = height;
    return convertedData;
}
