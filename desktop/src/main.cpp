#include "Game.h"
#include "Window.h"
#include "utils.h"
#include <stdlib.h>

void onResizeListener(int32_t width, int32_t height, float scale) {
    Game::getInstance()->resize(width, height, scale);
}

int main(int argc, char **argv) {
    Window window("Simple Native OpenGL Example", 1280, 720);
    window.setMinSize(320, 240);
    window.setOnResizeListener(onResizeListener);

    Game *game = Game::getInstance();
    game->resize(window.width, window.height, window.scale);
    game->init();

    double time = glfwGetTime();
    while (window.isRunning()) {
        window.pollEvents();

        double newTime = glfwGetTime();
        game->update(MIN(newTime - time, 1));
        time = newTime;

        game->render();
        window.present();
    }
    return EXIT_SUCCESS;
}
