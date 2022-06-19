#include "std.h"

JNIEnv *env;
jclass libGameClass;

void *operator new(size_t size) {
    return malloc(size);
}

void operator delete(void *ptr) noexcept {
    free(ptr);
}
