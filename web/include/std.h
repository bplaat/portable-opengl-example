#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// C standard library bindings
#define WASM_PAGE_SIZE 65536
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define M_PI 3.14159265358979323846

extern "C" {
extern int32_t printf(const char *format, ...);
extern int32_t vprintf(const char *format, va_list arg);
extern void exit(int32_t status);
extern double floor(double x);
extern double sin(double x);
extern double cos(double x);
extern double tan(double x);
extern uint32_t time(void *timer);

extern uint8_t __heap_base;
void *malloc(size_t size);
void free(void *ptr);
}

// C++ standard library wrappers
void *operator new(size_t size);
void operator delete(void *ptr) noexcept;
