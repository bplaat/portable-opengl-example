#pragma once

#include <stdint.h>

// OpenGL ES 3.0 bindings
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_TEST 0x0B71
#define GL_SCISSOR_TEST 0x0C11
#define GL_BLEND 0x0BE2
#define GL_CULL_FACE 0x0B44
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_LINES 0x0001
#define GL_TRIANGLES 0x0004
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_UNSIGNED_BYTE 0x1401

extern "C" {
extern char *glGetString(uint32_t name);
extern void glViewport(int32_t x, int32_t y, int32_t width, int32_t height);
extern void glClearColor(float red, float green, float blue, float alpha);
extern void glClear(uint32_t mask);
extern void glScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
extern void glEnable(uint32_t cap);
extern void glDisable(uint32_t cap);
extern void glBlendFunc(uint32_t sfactor, uint32_t dfactor);
extern void glCullFace(uint32_t mode);

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

extern void glGenTextures(int32_t n, uint32_t *textures);
extern void glBindTexture(uint32_t target, uint32_t texture);
extern void glTexParameteri(uint32_t target, uint32_t pname, int32_t param);
extern void glTexImage2D(uint32_t target, int32_t level, int32_t internalformat, int32_t width, int32_t height,
                         int32_t border, int32_t format, uint32_t type, const void *pixels);
extern void glGenerateMipmap(uint32_t target);
extern void glDeleteTextures(int32_t n, uint32_t *textures);
}
