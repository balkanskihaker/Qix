#pragma once
#include <string>

struct LevelConfig {
    int level;
    float qixSpeed;
    int stixSpeedBorder;
    int stixSpeedSlow;
    int stixSpeedFast;
    int sparxSpeed;
    int burnTimeFrames;    // 0 = nema sagorijevanja, else broj frejmova
    int sparxSpawnFrames;  // 0 = nema spawna, else interval u frejmovima
    float capturePercent;  // % potreban za prolaz
    bool bonusLife;
    std::string message;
};

extern LevelConfig currentLevel;
extern int levelMessageTimer;  // Koliko frejmova jos prikazujemo poruku

void levelManagerInit(int level);
void levelManagerUpdate();
LevelConfig getLevelConfig(int level);