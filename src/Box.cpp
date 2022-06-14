#include "Box.h"
#include "Game.h"
#include "bindings.h"
#include "utils.h"

Box::Box() {
    width = rand(10, 50);
    height = rand(10, 50);
    Game *game = Game::getInstance();
    x = rand(0, game->width - width);
    y = rand(0, game->height - height);
    vx = rand(-250, 250);
    vy = rand(-250, 250);
    color = rand(0, 0xffffff);
}

void Box::update(float delta) {
    x += vx * delta;
    y += vy * delta;
    Game *game = Game::getInstance();
    if (x < 0 || x + width > game->width) vx = -vx;
    if (y < 0 || y + height > game->height) vy = -vy;
}

void Box::render() {
    Game *game = Game::getInstance();
    glScissor(x * game->scale, y * game->scale, width * game->scale, height * game->scale);
    glClearColor((float)(color & 0xff) / 0xff, (float)((color >> 8) & 0xff) / 0xff, (float)((color >> 16) & 0xff) / 0xff, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}
