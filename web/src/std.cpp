#include "std.h"

uint32_t bumpPtr = (uint32_t)&__heap_base;

__attribute__((export_name("malloc"))) void *malloc(size_t size) {
    uint32_t nextBumpPtr = (bumpPtr + size) + (bumpPtr + size) % 4;
    uint32_t memSize = __builtin_wasm_memory_size(0) * WASM_PAGE_SIZE;
    if (nextBumpPtr > memSize) {
        __builtin_wasm_memory_grow(0, ((nextBumpPtr - memSize) + (WASM_PAGE_SIZE - 1)) / WASM_PAGE_SIZE);
    }

    uint32_t ptr = bumpPtr;
    bumpPtr = nextBumpPtr;
    return (void *)ptr;
}

__attribute__((export_name("free"))) void free(void *ptr) {
    (void)ptr;
    // Nothing to do here :)
}

void *operator new(size_t size) {
    return malloc(size);
}

void operator delete(void *ptr) noexcept {
    free(ptr);
}
