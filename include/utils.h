#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

extern uint8_t __heap_base;

void* malloc(size_t size);

void free(void *ptr);

void *operator new(size_t size);

int32_t rand(int32_t min, int32_t max);

#endif
