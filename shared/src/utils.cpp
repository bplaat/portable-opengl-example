#include "utils.h"
#include "bindings.h"

#ifdef PLATFORM_WEB

uint32_t bumpPtr = (uint32_t)&__heap_base;
void *malloc(size_t size) {
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

void operator delete(void *ptr) noexcept {
    free(ptr);
}

#endif
