#include "objects/Box.h"
#include "Game.h"
#include "math/Random.h"
#include "opengl.h"

Box::Box(Random *random) {
    width = random->randInt(10, 50);
    height = random->randInt(10, 50);
    Game *game = Game::getInstance();
    x = random->randInt((game->width - width) / 4, (game->width - width) / 4 * 3);
    y = random->randInt((game->height - height) / 4, (game->height - height) / 4 * 3);
    vx = random->randInt(-250, 250);
    vy = random->randInt(-250, 250);
    color = random->randInt(0, 0xffffff);
}

void Box::update(float delta) {
    x += vx * delta;
    y += vy * delta;
    Game *game = Game::getInstance();
    if (x < 0 || x + width > game->width) {
        vx = -vx;
    }
    if (y < 0 || y + height > game->height) {
        vy = -vy;
    }
}

void Box::render() {
    Game *game = Game::getInstance();
    glScissor(x * game->scale, (game->height - y - height) * game->scale, width * game->scale, height * game->scale);
    glClearColor((float)(color & 0xff) / 0xff, (float)((color >> 8) & 0xff) / 0xff,
                 (float)((color >> 16) & 0xff) / 0xff, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}
