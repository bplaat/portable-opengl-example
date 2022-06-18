#include "Window.h"
#include <stdio.h>
#include <stdlib.h>

void Window::sizeCallback(GLFWwindow *window, int width, int height) {
    Window *windowPtr = (Window *)glfwGetWindowUserPointer(window);
    windowPtr->width = width;
    windowPtr->height = height;
    if (windowPtr->onResizeListener != NULL) {
        windowPtr->onResizeListener(windowPtr->width, windowPtr->height, windowPtr->scale);
    }
}

void Window::contentScaleCallback(GLFWwindow *window, float scaleX, float scaleY) {
    (void)scaleX;
    Window *windowPtr = (Window *)glfwGetWindowUserPointer(window);
    windowPtr->scale = scaleY;
    if (windowPtr->onResizeListener != NULL) {
        windowPtr->onResizeListener(windowPtr->width, windowPtr->height, windowPtr->scale);
    }
}

Window::Window(const char *title, int32_t width, int32_t height)
    : title(title), width(width), height(height), onResizeListener(NULL) {
    // Init the GLFW3 library
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    atexit(glfwTerminate);

    // Select OpenGL 3.3 core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window and set some settings
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwSetWindowUserPointer(window, this);
    glfwGetWindowContentScale(window, &scale, NULL);
    glfwMakeContextCurrent(window);

    // Set window callback listeners
    glfwSetWindowSizeCallback(window, Window::sizeCallback);
    glfwSetWindowContentScaleCallback(window, Window::contentScaleCallback);

    // Center window
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *primaryScreen = glfwGetVideoMode(primaryMonitor);
    glfwSetWindowPos(window, (primaryScreen->width - width) / 2, (primaryScreen->height - height) / 2);

    // Load OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(window);
        exit(EXIT_FAILURE);
    }
}

void Window::setMinSize(int32_t minWidth, int32_t minHeight) {
    this->minWidth = minWidth;
    this->minHeight = minHeight;
    glfwSetWindowSizeLimits(window, minWidth, minHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);
}

void Window::setOnResizeListener(void (*onResizeListener)(int32_t width, int32_t height, float scale)) {
    this->onResizeListener = onResizeListener;
}

bool Window::isRunning() {
    return !glfwWindowShouldClose(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::present() {
    glfwSwapBuffers(window);
}

Window::~Window() {
    glfwDestroyWindow(window);
}
