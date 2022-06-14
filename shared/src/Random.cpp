#include "Random.h"
#include "bindings.h"

Random::Random() : seed(1) {}

Random::Random(uint64_t seed) : seed(seed) {}

double Random::random() {
    double x = sin(seed++) * 10000;
    return x - floor(x);
}

int32_t Random::randInt(int32_t min, int32_t max) {
    return floor(random() * (double)(max - min + 1)) + min;
}
