#include <GLUT/glut.h>
#include "menu.h"
#include "game.h"
#include "scoreboard.h"
#include "../core/renderer.h"
#include "../core/scene_manager.h"
#include "../core/gamestate.h"
#include "../utils/config.h"
#include <cstdlib>
#include <cstring>

static int selectedOption = 0;
static const int NUM_OPTIONS = 3;
static const char* options[] = { "START GAME", "SCOREBOARD", "QUIT" };

void menuInit() {
    selectedOption = 0;
}

void menuUpdate() {}

void menuRender() {
    const char* title = "Q I X";
    int titleLen = 5 * 8;
    drawText((WINDOW_WIDTH - titleLen) / 2, 150, title, MENU_TITLE_R, MENU_TITLE_G, MENU_TITLE_B);

    const char* subtitle = "ARCADE CLASSIC";
    int subtitleLen = 14 * 8;
    drawText((WINDOW_WIDTH - subtitleLen) / 2, 175, subtitle, COL_PINK_R, COL_PINK_G, COL_PINK_B);

    for (int i = 0; i < NUM_OPTIONS; i++) {
        float r, g, b;
        if (i == selectedOption) {
            r = MENU_SEL_R; g = MENU_SEL_G; b = MENU_SEL_B;
        } else {
            r = MENU_UNSEL_R; g = MENU_UNSEL_G; b = MENU_UNSEL_B;
        }

        int optLen = strlen(options[i]) * 8;
        int y = 260 + i * 40;

        if (i == selectedOption) {
            drawText((WINDOW_WIDTH - optLen) / 2 - 20, y, ">", MENU_SEL_R, MENU_SEL_G, MENU_SEL_B);
        }

        drawText((WINDOW_WIDTH - optLen) / 2, y, options[i], r, g, b);
    }

    const char* controls = "UP/DOWN to navigate, ENTER to select";
    int ctrlLen = strlen(controls) * 8;
    drawText((WINDOW_WIDTH - ctrlLen) / 2, 450, controls, MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
}

void menuHandleKey(int key) {
    if (key == GLUT_KEY_UP)   selectedOption = (selectedOption - 1 + NUM_OPTIONS) % NUM_OPTIONS;
    if (key == GLUT_KEY_DOWN) selectedOption = (selectedOption + 1) % NUM_OPTIONS;
}

void menuHandleKeyChar(unsigned char key) {
    if (key == 27) exit(0);
    if (key == 13) {
        switch (selectedOption) {
            case 0: switchScene(Scene::GAME);        break;
            case 1: switchScene(Scene::SCOREBOARD);  break;
            case 2: exit(0);                         break;
        }
    }
}
