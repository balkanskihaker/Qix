#include "scoreboard.h"
#include "../core/renderer.h"
#include "../core/scene_manager.h"
#include "../utils/config.h"
#include <fstream>
#include <algorithm>
#include <cstdio>

std::vector<ScoreEntry> scoreEntries;
std::string nameInput = "";
bool enteringName = false;

static const std::string SCORES_FILE = "data/scores.txt";

static void loadScores() {
    scoreEntries.clear();
    std::ifstream file(SCORES_FILE);
    if (!file.is_open()) return;

    std::string name;
    int score, level;
    while (file >> name >> score >> level) {
        scoreEntries.push_back({name, score, level});
    }
    file.close();

    std::sort(scoreEntries.begin(), scoreEntries.end(),
              [](const ScoreEntry& a, const ScoreEntry& b) {
                  return a.score > b.score;
              });

    if (scoreEntries.size() > 10) scoreEntries.resize(10);
}

static void saveScores() {
    std::ofstream file(SCORES_FILE);
    if (!file.is_open()) return;
    for (auto& e : scoreEntries)
        file << e.name << " " << e.score << " " << e.level << "\n";
    file.close();
}

void scoreboardAddScore(int score, int level) {
    loadScores();
    scoreEntries.push_back({nameInput, score, level});
    std::sort(scoreEntries.begin(), scoreEntries.end(),
              [](const ScoreEntry& a, const ScoreEntry& b) {
                  return a.score > b.score;
              });
    if (scoreEntries.size() > 10) scoreEntries.resize(10);
    saveScores();
}

void scoreboardInit() {
    loadScores();
    nameInput = "";
    enteringName = false;
}

void scoreboardUpdate() {}

void scoreboardRender() {
    char buf[64];

    int titleLen = 10 * 8;
    drawText((WINDOW_WIDTH - titleLen) / 2, 80, "SCOREBOARD",
             MSG_COL_R, MSG_COL_G, MSG_COL_B);

    drawText(200, 130, "RANK", MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
    drawText(280, 130, "NAME", MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
    drawText(380, 130, "SCORE", MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
    drawText(480, 130, "LEVEL", MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);

    for (int i = 0; i < (int)scoreEntries.size(); i++) {
        float r, g, b;
        if (i == 0) { r = MSG_COL_R;  g = MSG_COL_G;  b = MSG_COL_B;  }  // Zlato — zuta
        else if (i == 1) { r = COL_BLUE_R; g = COL_BLUE_G; b = COL_BLUE_B; }  // Srebro — plava
        else if (i == 2) { r = COL_PINK_R; g = COL_PINK_G; b = COL_PINK_B; }  // Bronza — pink
        else { r = HUD_COL_R; g = HUD_COL_G; b = HUD_COL_B; }

        int y = 160 + i * 30;
        sprintf(buf, "#%d", i + 1);
        drawText(200, y, buf, r, g, b);
        drawText(280, y, scoreEntries[i].name.c_str(), r, g, b);
        sprintf(buf, "%d", scoreEntries[i].score);
        drawText(380, y, buf, r, g, b);
        sprintf(buf, "%d", scoreEntries[i].level);
        drawText(480, y, buf, r, g, b);
    }

    int backLen = 20 * 8;
    drawText((WINDOW_WIDTH - backLen) / 2, 500, "Press R to play again",
             MENU_UNSEL_R, MENU_UNSEL_G, MENU_UNSEL_B);
}

void scoreboardHandleKeyChar(unsigned char key) {
    if (enteringName) {
        if ((key >= 'A' && key <= 'Z') || (key >= 'a' && key <= 'z')) {
            if (nameInput.length() < 10) nameInput += toupper(key);
        }
        if (key == 127 || key == 8) {
            if (nameInput.length() > 0) nameInput.pop_back();
        }
        if (key == 13 && nameInput.length() > 0) enteringName = false;
        return;
    }

    if (key == 27) switchScene(Scene::MENU);
    if (key == 'r' || key == 'R') switchScene(Scene::GAME);
}
