#include "Game.h"
#include "Box.h"
#include "Random.h"
#include "bindings.h"
#include "utils.h"

Game *Game::getInstance() {
    static Game game;
    return &game;
}

void Game::init() {
    Random random(time(NULL));

#ifdef PLATFORM_WEB
    // Grow heap with 2 * 64 KiB pages
    __builtin_wasm_memory_grow(0, 2);
#endif

    boxesSize = 4 * 1024;
    boxes = (Box **)malloc(boxesSize * sizeof(Box *));
    for (int32_t i = 0; i < boxesSize; i++) {
        boxes[i] = new Box(&random);
    }
}

void Game::resize(int32_t width, int32_t height, float scale) {
    this->width = width;
    this->height = height;
    this->scale = scale;
}

void Game::update(float delta) {
    for (int32_t i = 0; i < boxesSize; i++) {
        Box *box = boxes[i];
        box->update(delta);
    }
}

void Game::render() {
    glViewport(0, 0, width * scale, height * scale);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    for (int32_t i = 0; i < boxesSize; i++) {
        Box *box = boxes[i];
        box->render();
    }
    glDisable(GL_SCISSOR_TEST);
}
