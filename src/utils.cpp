#include "utils.h"
#include "bindings.h"

uint32_t bumpPtr = (uint32_t)&__heap_base;
void* malloc(size_t size) {
    uint32_t ptr = bumpPtr;
    bumpPtr += size;
    return (void *)ptr;
}

void free(void *ptr) {
    // Nothing to do here :)
}

void *operator new(size_t size) {
    return malloc(size);
}

int32_t rand(int32_t min, int32_t max) {
    return floor(random() * (double)(max - min + 1)) + min;
}
