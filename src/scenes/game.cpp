#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include "game.h"
#include "scoreboard.h"
#include "../core/grid.h"
#include "../core/renderer.h"
#include "../core/gamestate.h"
#include "../core/levelmanager.h"
#include "../core/scene_manager.h"
#include "../utils/config.h"
#include "../entities/stix.h"
#include "../entities/sparx.h"
#include "../entities/qix.h"

static bool wasDrawing = false;
static DrawMode lastDrawMode = DrawMode::FAST;

void gameInit() {
    gridInit(WINDOW_WIDTH, WINDOW_HEIGHT);
    gameStateInit();
    stixInit();
    sparxInit();
    qixInit();
    levelManagerInit(gameState.level);
    wasDrawing = false;
    nameInput = "";
    enteringName = false;
}

void gameUpdate() {
    if (isPaused) return;
    if (gameState.gameOver || gameState.levelComplete) return;

    if (levelMessageTimer > 0) {
        levelMessageTimer--;
        return;
    }

    if (gameState.invincibilityTimer > 0) gameState.invincibilityTimer--;

    if (stix.isDrawing) {
        burnTimer++;
    } else {
        burnTimer = 0; burnIndex = 0; isBurning = false;
    }

    if (currentLevel.burnTimeFrames > 0 && stix.isDrawing) {
        if (burnTimer >= currentLevel.burnTimeFrames) isBurning = true;

        if (isBurning) {
            burnIndex += currentLevel.stixSpeedSlow;
            if (burnIndex >= (int)stix.trail.size() - 1) {
                gameStateLoseLife();
                if (!gameState.gameOver) {
                    stixInit(); wasDrawing = false;
                } else {
                    enteringName = true; nameInput = "";
                }
                return;
            }
        }
    }

    levelManagerUpdate();

    if (!wasDrawing && stix.isDrawing) {
        wasDrawing = true;
        lastDrawMode = stix.drawMode;
    }

    if (wasDrawing && !stix.isDrawing) {
        wasDrawing = false;
        float pct = gridClaimedPercent();
        int multiplier = (lastDrawMode == DrawMode::SLOW) ? 2 : 1;
        int points = (int)(pct * 10 * multiplier);
        gameStateAddScore(points);

        if (pct >= currentLevel.capturePercent) {
            gameState.levelComplete = true;
            if (gameState.level >= 10) isVictory = true;
        }
    }

    stixUpdate();
    sparxUpdate();
    qixUpdate();

    if (sparxCollidesWithStix(stix.x, stix.y)) {
        gameStateLoseLife();
        if (!gameState.gameOver) { stixInit(); wasDrawing = false; }
        else { enteringName = true; nameInput = ""; }
        return;
    }

    if (qixCollidesWithTrail()) {
        gameStateLoseLife();
        if (!gameState.gameOver) { stixInit(); wasDrawing = false; }
        else { enteringName = true; nameInput = ""; }
        return;
    }
}

void gameRender() {
    gridRender();
    stixRender();
    sparxRender();
    qixRender();

    char buf[64];

    sprintf(buf, "SCORE: %d", gameState.score);
    drawText(BORDER_X, BORDER_Y / 2 + 5, buf, HUD_COL_R, HUD_COL_G, HUD_COL_B);

    sprintf(buf, "LIVES: %d", gameState.lives);
    drawText(WINDOW_WIDTH - BORDER_X - 80, BORDER_Y / 2 + 5, buf, HUD_COL_R, HUD_COL_G, HUD_COL_B);

    sprintf(buf, "LEVEL: %d", gameState.level);
    drawText(WINDOW_WIDTH / 2 - 30, BORDER_Y / 2 + 5, buf, HUD_COL_R, HUD_COL_G, HUD_COL_B);

    sprintf(buf, "%.0f%%", gridClaimedPercent());
    drawText(WINDOW_WIDTH / 2 - 15, WINDOW_HEIGHT - BORDER_Y / 2, buf, HUD_PCT_R, HUD_PCT_G, HUD_PCT_B);

    if (levelMessageTimer > 0) {
        int msgLen = currentLevel.message.length() * 8;
        int msgX = (WINDOW_WIDTH - msgLen) / 2;
        drawText(msgX, WINDOW_HEIGHT / 2 - 10,
                 currentLevel.message.c_str(), MSG_COL_R, MSG_COL_G, MSG_COL_B);
    }

    if (currentLevel.burnTimeFrames > 0 && stix.isDrawing && !isBurning) {
        int remaining = currentLevel.burnTimeFrames - burnTimer;
        if (remaining <= 2 * 60) {
            int msgLen = 13 * 8;
            int msgX = (WINDOW_WIDTH - msgLen) / 2;
            drawText(msgX, WINDOW_HEIGHT - BORDER_Y - 20,
                     "LINE BURNING!", STIX_BURN_R, STIX_BURN_G, STIX_BURN_B);
        }
    }

    if (gameState.gameOver) {
        if (enteringName) {
            int msgLen = 16 * 8;
            drawText((WINDOW_WIDTH - msgLen) / 2, WINDOW_HEIGHT / 2 - 40,
                     "ENTER YOUR NAME:", MSG_COL_R, MSG_COL_G, MSG_COL_B);

            std::string display = nameInput;
            if (nameInput.length() < 10) display += "_";
            int nameLen = display.length() * 8;
            drawText((WINDOW_WIDTH - nameLen) / 2, WINDOW_HEIGHT / 2,
                     display.c_str(), HUD_COL_R, HUD_COL_G, HUD_COL_B);

            int hintLen = 22 * 8;
            drawText((WINDOW_WIDTH - hintLen) / 2, WINDOW_HEIGHT / 2 + 30,
                     "Press ENTER to confirm", MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
        } else {
            int msgLen = 9 * 8;
            drawText((WINDOW_WIDTH - msgLen) / 2, WINDOW_HEIGHT / 2,
                     "GAME OVER", STIX_BURN_R, STIX_BURN_G, STIX_BURN_B);
            int restartLen = 18 * 8;
            drawText((WINDOW_WIDTH - restartLen) / 2, WINDOW_HEIGHT / 2 + 20,
                     "Press R to restart", HUD_COL_R, HUD_COL_G, HUD_COL_B);
            int boardLen = 22 * 8;
            drawText((WINDOW_WIDTH - boardLen) / 2, WINDOW_HEIGHT / 2 + 40,
                     "Press S for scoreboard", MSG_COL_R, MSG_COL_G, MSG_COL_B);
        }
    }

    if (gameState.levelComplete) {
        if (isVictory) {
            int msgLen = 16 * 8;
            drawText((WINDOW_WIDTH - msgLen) / 2, WINDOW_HEIGHT / 2 - 60,
                     "CONGRATULATIONS!", MSG_COL_R, MSG_COL_G, MSG_COL_B);

            sprintf(buf, "FINAL SCORE: %d", gameState.score);
            int scoreLen = strlen(buf) * 8;
            drawText((WINDOW_WIDTH - scoreLen) / 2, WINDOW_HEIGHT / 2 - 20,
                     buf, HUD_COL_R, HUD_COL_G, HUD_COL_B);

            int lvlLen = 23 * 8;
            drawText((WINDOW_WIDTH - lvlLen) / 2, WINDOW_HEIGHT / 2 + 20,
                     "YOU CONQUERED THE QIX!", COL_BLUE_R, COL_BLUE_G, COL_BLUE_B);

            int hintLen = 22 * 8;
            drawText((WINDOW_WIDTH - hintLen) / 2, WINDOW_HEIGHT / 2 + 60,
                     "Press S for scoreboard", MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
            int menuLen = 16 * 8;
            drawText((WINDOW_WIDTH - menuLen) / 2, WINDOW_HEIGHT / 2 + 80,
                     "Press M for menu", MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
        } else {
            int msgLen = 15 * 8;
            drawText((WINDOW_WIDTH - msgLen) / 2, WINDOW_HEIGHT / 2,
                     "LEVEL COMPLETE!", COL_BLUE_R, COL_BLUE_G, COL_BLUE_B);
            msgLen = 22 * 8;
            drawText((WINDOW_WIDTH - msgLen) / 2, WINDOW_HEIGHT / 2 + 20,
                     "Press N for next level", HUD_COL_R, HUD_COL_G, HUD_COL_B);
        }
    }

    if (isPaused) {
        int msgLen = 6 * 8;
        drawText((WINDOW_WIDTH - msgLen) / 2, WINDOW_HEIGHT / 2 - 20,
                 "PAUSED", HUD_COL_R, HUD_COL_G, HUD_COL_B);
        int resumeLen = 26 * 8;
        drawText((WINDOW_WIDTH - resumeLen) / 2, WINDOW_HEIGHT / 2 + 10,
                 "ESC to resume / M for menu", MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
        return;
    }
}

void gameHandleKey(int key) {
    stixHandleKey(key);
}

void gameHandleKeyChar(unsigned char key) {
    if (gameState.gameOver && enteringName) {
        if (key == 27) exit(0);
        scoreboardHandleKeyChar(key);
        if (!enteringName) scoreboardAddScore(gameState.score, gameState.level);
        return;
    }

    if (key == 27) {
        if (!gameState.gameOver && !gameState.levelComplete) {
            isPaused = !isPaused;
        }
        return;
    }

    if (key == 'm' || key == 'M') {
        if (isPaused) { isPaused = false; switchScene(Scene::MENU); return; }
        if (isVictory) { switchScene(Scene::MENU); return; }
    }

    if (key == 'p' || key == 'P') {
        if (!gameState.gameOver && !gameState.levelComplete) isPaused = !isPaused;
        return;
    }

    if (isPaused) return;

    if (gameState.gameOver) {
        if (key == 'r' || key == 'R') { gameState.level = 1; isPaused = false; gameInit(); return; }
        if (key == 's' || key == 'S') { switchScene(Scene::SCOREBOARD); return; }
        return;
    }

    if (gameState.levelComplete) {
        if (isVictory) {
            if (key == 's' || key == 'S') { switchScene(Scene::SCOREBOARD); return; }
            return;
        }
        if (key == 'n' || key == 'N') {
            int savedScore = gameState.score;
            int savedLives = gameState.lives;
            int nextLevel  = gameState.level + 1;
            bool bonusLife = currentLevel.bonusLife;
            gameInit();
            gameState.score = savedScore;
            gameState.lives = savedLives + (bonusLife ? 1 : 0);
            gameState.level = nextLevel;
            levelManagerInit(nextLevel);
            return;
        }
        return;
    }

    if (key == 'r' || key == 'R') { gameState.level = 1; isPaused = false; gameInit(); return; }

    stixHandleKeyChar(key);
}
