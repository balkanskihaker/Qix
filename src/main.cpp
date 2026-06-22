#include <GLUT/glut.h>
#include <cstdlib>
#include "core/scene_manager.h"
#include "scenes/menu.h"
#include "scenes/game.h"
#include "scenes/scoreboard.h"
#include "utils/config.h"
#include "core/renderer.h"
#include "entities/stix.h"

Scene currentScene = Scene::MENU;

void switchScene(Scene scene) {
    currentScene = scene;
    sceneInit(scene);
}

void sceneInit(Scene scene) {
    switch (scene) {
        case Scene::MENU:        menuInit();        break;
        case Scene::GAME:        gameInit();        break;
        case Scene::SCOREBOARD:  scoreboardInit();  break;
    }
}

void sceneUpdate(Scene scene) {
    switch (scene) {
        case Scene::MENU:        menuUpdate();        break;
        case Scene::GAME:        gameUpdate();        break;
        case Scene::SCOREBOARD:  scoreboardUpdate();  break;
    }
}

void sceneRender(Scene scene) {
    switch (scene) {
        case Scene::MENU:        menuRender();        break;
        case Scene::GAME:        gameRender();        break;
        case Scene::SCOREBOARD:  scoreboardRender();  break;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    sceneRender(currentScene);
    drawCRTEffect();
    glutSwapBuffers();
}

void update(int value) {
    sceneUpdate(currentScene);
    glutPostRedisplay();
    glutTimerFunc(1000 / TARGET_FPS, update, 0);
}

void keyboardGame(unsigned char key, int x, int y) {
    if (currentScene == Scene::GAME)        gameHandleKeyChar(key);
    if (currentScene == Scene::MENU)        menuHandleKeyChar(key);
    if (currentScene == Scene::SCOREBOARD)  scoreboardHandleKeyChar(key);
}

void specialKey(int key, int x, int y) {
    if (currentScene == Scene::GAME)  gameHandleKey(key);
    if (currentScene == Scene::MENU)  menuHandleKey(key);
}

void specialKeyRelease(int key, int x, int y) {
    if (currentScene == Scene::GAME) stixHandleKeyRelease(key);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    glClearColor(BG_R, BG_G, BG_B, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);

    sceneInit(currentScene);
    initCRTTexture();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardGame);
    glutSpecialFunc(specialKey);
    glutSpecialUpFunc(specialKeyRelease);
    glutTimerFunc(1000 / TARGET_FPS, update, 0);

    glutMainLoop();
    return 0;
}
