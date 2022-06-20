#pragma once

#include <jni.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern JNIEnv *env;
extern jclass libGameClass;

// C++ standard library wrappers
void *operator new(size_t size);
void operator delete(void *ptr) noexcept;
