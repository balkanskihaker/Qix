#pragma once

struct GameState {
    int lives;
    int score;
    int level;
    bool gameOver;
    bool levelComplete;
    int invincibilityTimer;
};


extern GameState gameState;
extern bool isPaused;
extern bool isVictory;

void gameStateInit();
void gameStateAddScore(int points);
void gameStateLoseLife();
float gameStateClaimedPercent();