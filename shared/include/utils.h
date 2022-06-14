#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

#ifdef PLATFORM_WEB

extern uint8_t __heap_base;

void *malloc(size_t size);

void free(void *ptr);

void *operator new(size_t size);

#endif

#endif
