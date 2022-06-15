#ifndef BINDINGS_H
#define BINDINGS_H

#include <stddef.h>
#include <stdint.h>

#ifdef PLATFORM_DESKTOP
    #include <glad/glad.h>
    #include <math.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>

void print(const char *string);
void println(const char *string);
#endif

#ifdef PLATFORM_WEB

extern "C" {

    // C standard library bindings
    #define EXIT_SUCCESS 0
    #define EXIT_FAILURE 1
    #define M_PI 3.14159265358979323846

extern void print(const char *string);
extern void println(const char *string);
extern void exit(int32_t status);
extern double floor(double x);
extern double sin(double x);
extern double cos(double x);
extern double tan(double x);
extern uint32_t time(void *timer);

extern uint8_t __heap_base;
void *malloc(size_t size);
void free(void *ptr);

    // OpenGL ES 3.0 bindings
    #define GL_RENDERER 0x1F01
    #define GL_VERSION 0x1F02
    #define GL_DEPTH_BUFFER_BIT 0x00000100
    #define GL_COLOR_BUFFER_BIT 0x00004000
    #define GL_SCISSOR_TEST 0x0C11
    #define GL_FRAGMENT_SHADER 0x8B30
    #define GL_VERTEX_SHADER 0x8B31
    #define GL_COMPILE_STATUS 0x8B81
    #define GL_ARRAY_BUFFER 0x8892
    #define GL_STATIC_DRAW 0x88E4
    #define GL_FLOAT 0x1406
    #define GL_FALSE 0
    #define GL_TRUE 1
    #define GL_TRIANGLES 0x0004

extern char *glGetString(uint32_t name);
extern void glViewport(int32_t x, int32_t y, int32_t width, int32_t height);
extern void glClearColor(float red, float green, float blue, float alpha);
extern void glClear(uint32_t mask);
extern void glScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
extern void glEnable(uint32_t cap);
extern void glDisable(uint32_t cap);

extern uint32_t glCreateShader(uint32_t shaderType);
extern void glShaderSource(uint32_t shader, int32_t count, const char **string, const int32_t *length);
extern void glCompileShader(uint32_t shader);
extern void glGetShaderiv(uint32_t shader, uint32_t pname, int32_t *params);
extern void glGetShaderInfoLog(uint32_t shader, uint32_t maxLength, uint32_t *length, char *infoLog);
extern void glDeleteShader(uint32_t shader);

extern uint32_t glCreateProgram(void);
extern void glAttachShader(uint32_t program, uint32_t shader);
extern void glLinkProgram(uint32_t program);
extern void glUseProgram(uint32_t program);

extern void glGenVertexArrays(int32_t n, uint32_t *arrays);
extern void glBindVertexArray(uint32_t array);

extern void glGenBuffers(int32_t n, uint32_t *buffers);
extern void glBindBuffer(uint32_t target, uint32_t buffer);
extern void glBufferData(uint32_t target, uint32_t size, const void *data, uint32_t usage);

extern int32_t glGetUniformLocation(uint32_t program, const char *name);
extern void glUniformMatrix4fv(int32_t location, int32_t count, uint32_t transpose, const float *value);
extern int32_t glGetAttribLocation(uint32_t program, const char *name);
extern void glVertexAttribPointer(uint32_t index, int32_t size, uint32_t type, uint32_t normalized, int32_t stride,
                                  const void *pointer);
extern void glEnableVertexAttribArray(uint32_t index);
extern void glDrawArrays(uint32_t mode, int32_t first, int32_t count);
}

// C++ standard library bindings
void *operator new(size_t size);
void operator delete(void *ptr) noexcept;

#endif

#endif
