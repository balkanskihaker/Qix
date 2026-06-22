#pragma once
#include <string>
#include <vector>

const int MAX_NAME_LENGTH = 10;

struct ScoreEntry {
    std::string name;
    int score;
    int level;
};

extern std::vector<ScoreEntry> scoreEntries;
extern std::string nameInput;
extern bool enteringName;

void scoreboardInit();
void scoreboardUpdate();
void scoreboardRender();
void scoreboardAddScore(int score, int level);
void scoreboardHandleKeyChar(unsigned char key);