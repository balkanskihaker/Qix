#include "levelmanager.h"
#include "../entities/sparx.h"
#include "../core/gamestate.h"
#include "../utils/config.h"
#include <cstring>

LevelConfig currentLevel;
int levelMessageTimer = 0;

static int sparxSpawnTimer = 0;

// Spawn novog Sparxa sto dalje od Stixa
extern int stixX();  // forward declaration — koristimo stix.x direktno
#include "../entities/stix.h"

static void spawnNewSparx() {
    // Pronadji poziciju na borderu sto dalje od Stixa
    int bestX = BORDER_X, bestY = BORDER_Y;
    int bestDist = 0;

    // Provjeri sve border pozicije
    // Gornja ivica
    for (int x = BORDER_X; x <= WINDOW_WIDTH - BORDER_X; x++) {
        int d = abs(x - stix.x) + abs(BORDER_Y - stix.y);
        if (d > bestDist) { bestDist = d; bestX = x; bestY = BORDER_Y; }
    }
    // Donja ivica
    for (int x = BORDER_X; x <= WINDOW_WIDTH - BORDER_X; x++) {
        int d = abs(x - stix.x) + abs((WINDOW_HEIGHT - BORDER_Y - 1) - stix.y);
        if (d > bestDist) { bestDist = d; bestX = x; bestY = WINDOW_HEIGHT - BORDER_Y - 1; }
    }
    // Leva ivica
    for (int y = BORDER_Y; y <= WINDOW_HEIGHT - BORDER_Y - 1; y++) {
        int d = abs(BORDER_X - stix.x) + abs(y - stix.y);
        if (d > bestDist) { bestDist = d; bestX = BORDER_X; bestY = y; }
    }
    // Desna ivica
    for (int y = BORDER_Y; y <= WINDOW_HEIGHT - BORDER_Y - 1; y++) {
        int d = abs((WINDOW_WIDTH - BORDER_X - 1) - stix.x) + abs(y - stix.y);
        if (d > bestDist) { bestDist = d; bestX = WINDOW_WIDTH - BORDER_X - 1; bestY = y; }
    }

    sparxAddNew(bestX, bestY, 1, 0);
}

LevelConfig getLevelConfig(int level) {
    LevelConfig cfg;
    cfg.level            = level;
    cfg.qixSpeed         = 1.5f;
    cfg.stixSpeedBorder  = 2;
    cfg.stixSpeedSlow    = 1;
    cfg.stixSpeedFast    = 3;
    cfg.sparxSpeed       = 0;
    cfg.burnTimeFrames   = 0;
    cfg.sparxSpawnFrames = 0;
    cfg.capturePercent   = 75.0f;
    cfg.bonusLife        = false;
    cfg.message          = "";

    switch (level) {
        case 1:
            cfg.message = "LEVEL 1: CLAIM 75% OF THE AREA!";
            break;
        case 2:
            cfg.burnTimeFrames = 3 * 60;  // 3s * 60fps
            cfg.message = "LEVEL 2: DRAWING LINE STARTS TO BURN AFTER 3S!";
            break;
        case 3:
            cfg.burnTimeFrames = 3 * 60;
            cfg.qixSpeed = 2.0f;
            cfg.message = "LEVEL 3: QIX GOT FASTER!";
            break;
        case 4:
            cfg.burnTimeFrames = 3 * 60;
            cfg.qixSpeed = 2.0f;
            cfg.sparxSpawnFrames = 60 * 60;  // 60s
            cfg.bonusLife = true;
            cfg.message = "LEVEL 4: NEW SPARX EVERY 60S! +1 LIFE!";
            break;
        case 5:
            cfg.burnTimeFrames = 3 * 60;
            cfg.qixSpeed = 2.0f;
            cfg.sparxSpawnFrames = 60 * 60;
            cfg.stixSpeedFast = 2;
            cfg.message = "LEVEL 5: FAST DRAWING IS SLOWER NOW!";
            break;
        case 6:
            cfg.burnTimeFrames = 1 * 60;  // 1s
            cfg.qixSpeed = 2.0f;
            cfg.sparxSpawnFrames = 60 * 60;
            cfg.stixSpeedFast = 2;
            cfg.message = "LEVEL 6: LINE BURNS FASTER!";
            break;
        case 7:
            cfg.burnTimeFrames = 1 * 60;
            cfg.qixSpeed = 2.0f;
            cfg.sparxSpawnFrames = 30 * 60;  // 30s
            cfg.stixSpeedFast = 2;
            cfg.message = "LEVEL 7: SPARX SPAWNS EVERY 30S!";
            break;
        case 8:
            cfg.burnTimeFrames = 1 * 60;
            cfg.qixSpeed = 2.0f;
            cfg.sparxSpawnFrames = 30 * 60;
            cfg.stixSpeedFast = 2;
            cfg.stixSpeedBorder = 1;
            cfg.bonusLife = true;
            cfg.message = "LEVEL 8: STIX IS SLOWER ON BORDER! +1 LIFE!";
            break;
        case 9:
            cfg.burnTimeFrames = 1 * 60;
            cfg.qixSpeed = 2.5f;
            cfg.sparxSpawnFrames = 30 * 60;
            cfg.stixSpeedFast = 4;
            cfg.stixSpeedBorder = 1;
            cfg.message = "LEVEL 9: QIX IS EVEN FASTER!";
            break;
        default:  // Level 10+
            cfg.burnTimeFrames = 1 * 60;
            cfg.qixSpeed = 2.5f;
            cfg.sparxSpawnFrames = 30 * 60;
            cfg.stixSpeedFast = 2;
            cfg.stixSpeedBorder = 1;
            cfg.capturePercent = 85.0f;
            cfg.message = "FINAL LEVEL: CAPTURE 85%!";
            break;
    }

    return cfg;
}

void levelManagerInit(int level) {
    currentLevel = getLevelConfig(level);
    sparxSpawnTimer = 0;
    levelMessageTimer = 3 * 60;  // 3s umesto 5s
}

void levelManagerUpdate() {
    if (levelMessageTimer > 0) levelMessageTimer--;

    // Sparx spawn timer
    if (currentLevel.sparxSpawnFrames > 0) {
        sparxSpawnTimer++;
        if (sparxSpawnTimer >= currentLevel.sparxSpawnFrames) {
            sparxSpawnTimer = 0;
            spawnNewSparx();
        }
    }
}