#ifndef GAME_H
#define GAME_H

#include <stddef.h>
#include <stdint.h>
class Game;
#include "Box.h"

class Game {
  public:
    int32_t width;
    int32_t height;
    float scale;

  private:
    Box **boxes;
    size_t boxesSize;

  public:
    static Game *getInstance();

    void init();

    void resize(int32_t width, int32_t height, float scale);

    void update(float delta);

    void render();
};

#endif
