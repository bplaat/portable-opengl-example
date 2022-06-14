#include "Game.h"

__attribute__((export_name("init")))
void init() {
    Game::getInstance()->init();
}

__attribute__((export_name("resize")))
void resize(int32_t width, int32_t height, float scale) {
    Game::getInstance()->resize(width, height, scale);
}

__attribute__((export_name("update")))
void update(float delta) {
    Game::getInstance()->update(delta);
}

__attribute__((export_name("render")))
void render() {
    Game::getInstance()->render();
}
