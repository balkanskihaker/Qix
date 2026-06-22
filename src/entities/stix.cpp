#include "stix.h"
#include "../entities/qix.h"
#include "../core/grid.h"
#include "../core/renderer.h"
#include "../core/levelmanager.h"
#include "../core/gamestate.h"
#include "../utils/config.h"
#include <GLUT/glut.h>

Stix stix;
int burnTimer = 0;
int burnIndex = 0;
bool isBurning = false;

void stixInit() {
    stix.x = WINDOW_WIDTH / 2;
    stix.y = BORDER_Y;
    stix.drawMode = DrawMode::NONE;
    stix.isDrawing = false;
    stix.trail.clear();
    stix.primaryKey = -1;
    for (int i = 0; i < 4; i++) stix.keyPressed[i] = false;
    burnTimer = 0;
    burnIndex = 0;
    isBurning = false;
}

static bool isInTrail(int x, int y) {
    for (auto& p : stix.trail)
        if (p.x == x && p.y == y) return true;
    return false;
}

static void closeLine() {
    for (auto& p : stix.trail) {
        gridSet(p.x, p.y, CellState::WALL);
    }

    int mid = stix.trail.size() / 2;
    int seed1x = -1, seed1y = -1;

    for (int i = mid; i < (int)stix.trail.size(); i++) {
        auto& p = stix.trail[i];
        int candidates[4][2] = {
            {p.x+1,p.y},{p.x-1,p.y},{p.x,p.y+1},{p.x,p.y-1}
        };
        for (auto& c : candidates) {
            if (gridGet(c[0], c[1]) == CellState::FREE) {
                seed1x = c[0]; seed1y = c[1];
                break;
            }
        }
        if (seed1x != -1) break;
    }

    if (seed1x == -1) {
        stix.trail.clear();
        stix.isDrawing = false;
        stix.drawMode = DrawMode::NONE;
        burnTimer = 0; burnIndex = 0; isBurning = false;
        return;
    }

    floodFill(seed1x, seed1y, CellState::TEMP);

    int seed2x = -1, seed2y = -1;
    for (auto& p : stix.trail) {
        int candidates[4][2] = {
            {p.x+1,p.y},{p.x-1,p.y},{p.x,p.y+1},{p.x,p.y-1}
        };
        for (auto& c : candidates) {
            if (gridGet(c[0], c[1]) == CellState::FREE) {
                seed2x = c[0]; seed2y = c[1];
                break;
            }
        }
        if (seed2x != -1) break;
    }

    if (seed2x == -1) {
        for (auto& row : grid)
            for (auto& cell : row)
                if (cell == CellState::TEMP) cell = CellState::CLAIMED;
    } else {
        int count1 = 0;
        for (auto& row : grid)
            for (auto& cell : row)
                if (cell == CellState::TEMP) count1++;

        floodFill(seed2x, seed2y, CellState::TEMP2);

        int count2 = 0;
        for (auto& row : grid)
            for (auto& cell : row)
                if (cell == CellState::TEMP2) count2++;

        int qx = (int)round(qix.x);
        int qy = (int)round(qix.y);
        CellState qixSide = gridGet(qx, qy);

        if (qixSide == CellState::TEMP) {
            for (auto& row : grid)
                for (auto& cell : row) {
                    if (cell == CellState::TEMP)  cell = CellState::FREE;
                    if (cell == CellState::TEMP2) cell = CellState::CLAIMED;
                }
        } else if (qixSide == CellState::TEMP2) {
            for (auto& row : grid)
                for (auto& cell : row) {
                    if (cell == CellState::TEMP)  cell = CellState::CLAIMED;
                    if (cell == CellState::TEMP2) cell = CellState::FREE;
                }
        } else {
            if (count1 <= count2) {
                for (auto& row : grid)
                    for (auto& cell : row) {
                        if (cell == CellState::TEMP)  cell = CellState::CLAIMED;
                        if (cell == CellState::TEMP2) cell = CellState::FREE;
                    }
            } else {
                for (auto& row : grid)
                    for (auto& cell : row) {
                        if (cell == CellState::TEMP)  cell = CellState::FREE;
                        if (cell == CellState::TEMP2) cell = CellState::CLAIMED;
                    }
            }
        }
    }

    stix.trail.clear();
    stix.isDrawing = false;
    stix.drawMode = DrawMode::NONE;
    burnTimer = 0; burnIndex = 0; isBurning = false;
}

static void tryMove(int dx, int dy) {
    int nx = stix.x + dx;
    int ny = stix.y + dy;

    if (nx < BORDER_X) nx = BORDER_X;
    if (ny < BORDER_Y) ny = BORDER_Y;
    if (nx > WINDOW_WIDTH  - BORDER_X - 1) nx = WINDOW_WIDTH  - BORDER_X - 1;
    if (ny > WINDOW_HEIGHT - BORDER_Y - 1) ny = WINDOW_HEIGHT - BORDER_Y - 1;

    bool currentOnBorder = gridIsOnBorder(stix.x, stix.y);
    bool nextOnBorder    = gridIsOnBorder(nx, ny);

    if (!stix.isDrawing) {
        if (nextOnBorder) {
            stix.x = nx; stix.y = ny;
        } else if (currentOnBorder && stix.drawMode != DrawMode::NONE) {
            stix.isDrawing = true;
            stix.trail.clear();
            stix.trail.push_back({stix.x, stix.y});
            stix.x = nx; stix.y = ny;
            stix.trail.push_back({stix.x, stix.y});
            burnTimer = 0; burnIndex = 0; isBurning = false;
        }
    } else {
        if (isInTrail(nx, ny)) {
            Point startPos = stix.trail[0];
            stix.x = startPos.x; stix.y = startPos.y;
            stix.trail.clear();
            stix.isDrawing = false;
            stix.drawMode = DrawMode::NONE;
            burnTimer = 0; burnIndex = 0; isBurning = false;
            gameStateLoseLife();
            return;
        }

        if (gridGet(nx, ny) == CellState::CLAIMED) return;

        if (nextOnBorder) {
            stix.trail.push_back({nx, ny});
            stix.x = nx; stix.y = ny;
            closeLine();
        } else {
            stix.x = nx; stix.y = ny;
            stix.trail.push_back({stix.x, stix.y});
        }
    }
}

static int keyToIndex(int key) {
    switch (key) {
        case GLUT_KEY_UP:    return 0;
        case GLUT_KEY_DOWN:  return 1;
        case GLUT_KEY_LEFT:  return 2;
        case GLUT_KEY_RIGHT: return 3;
        default: return -1;
    }
}

static int indexToKey(int idx) {
    switch (idx) {
        case 0: return GLUT_KEY_UP;
        case 1: return GLUT_KEY_DOWN;
        case 2: return GLUT_KEY_LEFT;
        case 3: return GLUT_KEY_RIGHT;
        default: return -1;
    }
}

static void applyKey(int key) {
    switch (key) {
        case GLUT_KEY_UP:    tryMove( 0, -1); break;
        case GLUT_KEY_DOWN:  tryMove( 0,  1); break;
        case GLUT_KEY_LEFT:  tryMove(-1,  0); break;
        case GLUT_KEY_RIGHT: tryMove( 1,  0); break;
    }
}

void stixUpdate() {
    if (stix.primaryKey == -1) return;

    int ticks = !stix.isDrawing ? currentLevel.stixSpeedBorder :
                (stix.drawMode == DrawMode::SLOW) ? currentLevel.stixSpeedSlow :
                currentLevel.stixSpeedFast;

    for (int i = 0; i < ticks; i++) {
        int prevX = stix.x, prevY = stix.y;
        applyKey(stix.primaryKey);
        if (stix.x == prevX && stix.y == prevY) {
            for (int j = 0; j < 4; j++) {
                if (stix.keyPressed[j] && indexToKey(j) != stix.primaryKey) {
                    applyKey(indexToKey(j));
                    if (stix.x != prevX || stix.y != prevY) break;
                }
            }
        }
    }
}

void stixRender() {
    if (stix.trail.size() > 1) {
        for (int i = 0; i < (int)stix.trail.size() - 1; i++) {
            float r = 0.0f, g = 0.0f, b = 0.0f;

            if (isBurning && i < burnIndex) {
                drawLine(stix.trail[i].x, stix.trail[i].y,
                         stix.trail[i+1].x, stix.trail[i+1].y,
                         STIX_BURNED_R, STIX_BURNED_G, STIX_BURNED_B, 2.0f);
                continue;
            } else if (isBurning && i == burnIndex) {
                r = STIX_BURN_R; g = STIX_BURN_G; b = STIX_BURN_B;
            } else if (isBurning) {
                int frame = glutGet(GLUT_ELAPSED_TIME) / 100;
                r = STIX_BURN_R;
                g = (frame % 2 == 0) ? 0.3f : 0.0f;
                b = STIX_BURN_B;
            } else if (currentLevel.burnTimeFrames > 0 && stix.isDrawing &&
                       burnTimer >= currentLevel.burnTimeFrames - 2 * 60) {
                int frame = glutGet(GLUT_ELAPSED_TIME) / 150;
                r = STIX_BURN_R;
                g = (frame % 2 == 0) ? 0.5f : 0.0f;
                b = STIX_BURN_B;
            } else {
                if (stix.drawMode == DrawMode::SLOW) {
                    r = STIX_SLOW_R; g = STIX_SLOW_G; b = STIX_SLOW_B;
                } else {
                    r = STIX_FAST_R; g = STIX_FAST_G; b = STIX_FAST_B;
                }
            }

            drawLine(stix.trail[i].x, stix.trail[i].y,
                     stix.trail[i+1].x, stix.trail[i+1].y,
                     r, g, b, 2.0f);
        }
    }

    drawRect(stix.x - 3, stix.y - 3, 6, 6, STIX_COL_R, STIX_COL_G, STIX_COL_B);
}

void stixHandleKey(int key) {
    int idx = keyToIndex(key);
    if (idx == -1) return;
    stix.keyPressed[idx] = true;
    stix.primaryKey = key;
}

void stixHandleKeyRelease(int key) {
    int idx = keyToIndex(key);
    if (idx == -1) return;
    stix.keyPressed[idx] = false;
    if (stix.primaryKey == key) {
        stix.primaryKey = -1;
        for (int i = 0; i < 4; i++) {
            if (stix.keyPressed[i]) {
                stix.primaryKey = indexToKey(i);
                break;
            }
        }
    }
}

void stixHandleKeyChar(unsigned char key) {
    if (stix.isDrawing) return;
    if (key == 's' || key == 'S') stix.drawMode = DrawMode::SLOW;
    if (key == 'f' || key == 'F') stix.drawMode = DrawMode::FAST;
}
