#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>

class Window {
  public:
    const char *title;
    int32_t width;
    int32_t height;
    int32_t minWidth;
    int32_t minHeight;
    float scale;
    GLFWwindow *window;
    void (*onResizeListener)(int32_t width, int32_t height, float scale);

    static void sizeCallback(GLFWwindow *window, int width, int height);

    static void contentScaleCallback(GLFWwindow *window, float scaleX, float scaleY);

    Window(const char *title, int32_t width, int32_t height);

    ~Window();

    void setMinSize(int32_t minWidth, int32_t minHeight);

    void setOnResizeListener(void (*onResizeListener)(int32_t width, int32_t height, float scale));

    bool isRunning();

    void pollEvents();

    void present();
};
