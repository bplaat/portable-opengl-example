#include "math/utils.h"
#include "bindings.h"

float degrees(float radians) {
    return radians * 180 / M_PI;
}

float radians(float degrees) {
    return degrees * M_PI / 180;
}
