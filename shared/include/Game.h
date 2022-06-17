#pragma once

#include <stddef.h>
#include <stdint.h>
class Game;
#include "math/Random.h"
#include "objects/Box.h"
#include "objects/PerspectiveCamera.h"
#include "textures/AssetTexture.h"
#include "textures/TextTexture.h"

class Game {
  public:
    int32_t width;
    int32_t height;
    float scale;

    Random *random;
    uint32_t program;
    uint32_t planeVertrexArray;
    uint32_t cubeVertrexArray;
    uint32_t matrixUniform;
    uint32_t cameraUniform;
    uint32_t textureUniform;
    Font *textFont;
    AssetTexture *crateTexture;
    AssetTexture *treeTexture;
    TextTexture *textTexture;
    PerspectiveCamera *camera;

    Box **boxes;
    size_t boxesSize;
    float rotation;

    static Game *getInstance();

    void init();

    void resize(int32_t width, int32_t height, float scale);

    void update(float delta);

    void render();
};
