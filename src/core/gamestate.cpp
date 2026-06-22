#include "gamestate.h"
#include "grid.h"
#include <cstdio>

GameState gameState;

const int INITIAL_LIVES = 3;
const int BONUS_LIFE_THRESHOLD = 10000;
bool isPaused = false;
bool isVictory = false;

void gameStateInit() {
    gameState.lives              = INITIAL_LIVES;
    gameState.score              = 0;
    gameState.level              = 1;
    gameState.gameOver           = false;
    gameState.levelComplete      = false;
    gameState.invincibilityTimer = 0;
    isPaused  = false;
    isVictory = false;
}

void gameStateAddScore(int points) {
    int prevScore = gameState.score;
    gameState.score += points;

    if (prevScore / BONUS_LIFE_THRESHOLD < gameState.score / BONUS_LIFE_THRESHOLD) {
        gameState.lives++;
    }
}

void gameStateLoseLife() {
    if (gameState.invincibilityTimer > 0) return;
    gameState.lives--;

    gameState.invincibilityTimer = 120;

    if (gameState.lives <= 0) {
        gameState.lives = 0;
        gameState.gameOver = true;
    }
}

float gameStateClaimedPercent() {
    return gridClaimedPercent();
}