#ifndef BOX_H
#define BOX_H

#include "math/Random.h"
#include <stdint.h>

class Box {
  private:
    float x;
    float y;
    float width;
    float height;
    float vx;
    float vy;
    uint32_t color;

  public:
    Box(Random *random);

    void update(float delta);

    void render();
};

#endif
