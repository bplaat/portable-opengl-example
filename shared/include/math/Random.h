#pragma once
#include <stdint.h>

class Random {
  public:
    uint64_t seed;

    Random();

    Random(uint64_t seed);

    double random();

    int32_t randInt(int32_t min, int32_t max);
};
