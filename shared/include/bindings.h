#ifndef BINDINGS_H
#define BINDINGS_H

#include <stddef.h>
#include <stdint.h>

#ifdef PLATFORM_DESKTOP
    #include <glad/glad.h>
    #include <stdio.h>
    #include <math.h>
    #include <time.h>
#endif

#ifdef PLATFORM_WEB
extern "C" {

extern double floor(double x);
extern double sin(double x);
extern uint32_t time(void *timer);

    #define GL_COLOR_BUFFER_BIT 0x00004000
    #define GL_SCISSOR_TEST 0x0C11

extern void glViewport(int32_t x, int32_t y, int32_t width, int32_t height);
extern void glClearColor(float red, float green, float blue, float alpha);
extern void glClear(uint32_t mask);
extern void glScissor(int32_t x, int32_t y, int32_t width, int32_t height);
extern void glEnable(uint32_t cap);
extern void glDisable(uint32_t cap);
}
#endif

#endif
