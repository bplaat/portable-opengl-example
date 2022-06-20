#include "Game.h"
#include "com_example_portablegl_LibGame.h"
#include "std.h"

JNIEXPORT void JNICALL Java_com_example_portablegl_LibGame_init(JNIEnv *_env, jclass _libGameClass) {
    env = _env;
    libGameClass = (jclass)env->NewGlobalRef(_libGameClass);
    Game::getInstance()->init();
}

JNIEXPORT void JNICALL Java_com_example_portablegl_LibGame_onResize(JNIEnv *env, jclass libGameClass, jint width,
                                                                    jint height, jfloat scale) {
    (void)env;
    (void)libGameClass;
    Game::getInstance()->onResize(width, height, scale);
}

JNIEXPORT void JNICALL Java_com_example_portablegl_LibGame_update(JNIEnv *env, jclass libGameClass, jfloat delta) {
    (void)env;
    (void)libGameClass;
    Game::getInstance()->update(delta);
}

JNIEXPORT void JNICALL Java_com_example_portablegl_LibGame_render(JNIEnv *env, jclass libGameClass) {
    (void)env;
    (void)libGameClass;
    Game::getInstance()->render();
}
