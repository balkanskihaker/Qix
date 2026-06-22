#include "sparx.h"
#include "stix.h"
#include "../core/grid.h"
#include "../core/renderer.h"
#include "../utils/config.h"
#include <GLUT/glut.h>
#include <cmath>
#include <cstdlib>

std::vector<SparxEntity> sparxList;

static bool isOnEdge(int x, int y) {
    if (x < BORDER_X || x > WINDOW_WIDTH  - BORDER_X - 1 ||
        y < BORDER_Y || y > WINDOW_HEIGHT - BORDER_Y - 1)
        return false;
    return gridGet(x, y) == CellState::BORDER || gridGet(x, y) == CellState::WALL;
}

static void generateWishlist(SparxEntity& s) {
    int possible[3][2] = {
        { s.dirX,  s.dirY},
        { s.dirY, -s.dirX},
        {-s.dirY,  s.dirX}
    };

    if (stix.isDrawing) {
        for (int i = 2; i > 0; i--) {
            int j = rand() % (i + 1);
            int tmpX = possible[i][0], tmpY = possible[i][1];
            possible[i][0] = possible[j][0]; possible[i][1] = possible[j][1];
            possible[j][0] = tmpX;           possible[j][1] = tmpY;
        }
    } else {
        int cx = (int)round(s.x);
        int cy = (int)round(s.y);
        int scores[3];
        for (int i = 0; i < 3; i++) {
            int tx = cx + possible[i][0];
            int ty = cy + possible[i][1];
            scores[i] = abs(tx - stix.x) + abs(ty - stix.y);
        }
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2 - i; j++) {
                if (scores[j] > scores[j+1]) {
                    int tmpX = possible[j][0], tmpY = possible[j][1];
                    possible[j][0] = possible[j+1][0]; possible[j][1] = possible[j+1][1];
                    possible[j+1][0] = tmpX;           possible[j+1][1] = tmpY;
                    int tmpS = scores[j]; scores[j] = scores[j+1]; scores[j+1] = tmpS;
                }
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        s.wishlist[i][0] = possible[i][0];
        s.wishlist[i][1] = possible[i][1];
    }
}

static void moveSparx(SparxEntity& s) {
    int cx = (int)round(s.x);
    int cy = (int)round(s.y);

    int leftX  =  s.dirY, leftY  = -s.dirX;
    int rightX = -s.dirY, rightY =  s.dirX;

    bool canGoStraight = isOnEdge(cx + s.dirX, cy + s.dirY);
    bool canGoLeft     = isOnEdge(cx + leftX,  cy + leftY);
    bool canGoRight    = isOnEdge(cx + rightX, cy + rightY);

    bool isIntersection = canGoLeft || canGoRight;

    if (isIntersection) {
        generateWishlist(s);
        for (int i = 0; i < 3; i++) {
            int tx = cx + s.wishlist[i][0];
            int ty = cy + s.wishlist[i][1];
            if (isOnEdge(tx, ty)) {
                s.dirX = s.wishlist[i][0];
                s.dirY = s.wishlist[i][1];
                s.x = tx; s.y = ty;
                return;
            }
        }
    }

    if (canGoStraight) {
        s.x = cx + s.dirX;
        s.y = cy + s.dirY;
        return;
    }

    generateWishlist(s);
    for (int i = 0; i < 3; i++) {
        int tx = cx + s.wishlist[i][0];
        int ty = cy + s.wishlist[i][1];
        if (isOnEdge(tx, ty)) {
            s.dirX = s.wishlist[i][0];
            s.dirY = s.wishlist[i][1];
            s.x = tx; s.y = ty;
            return;
        }
    }
}

void sparxInit() {
    sparxList.clear();
    srand(42);

    SparxEntity s1;
    s1.x = WINDOW_WIDTH / 2;
    s1.y = WINDOW_HEIGHT - BORDER_Y - 1;
    s1.dirX = -1; s1.dirY = 0;
    s1.speed = 1.0f;
    generateWishlist(s1);
    sparxList.push_back(s1);

    SparxEntity s2;
    s2.x = WINDOW_WIDTH / 2;
    s2.y = WINDOW_HEIGHT - BORDER_Y - 1;
    s2.dirX = 1; s2.dirY = 0;
    s2.speed = 1.0f;
    generateWishlist(s2);
    sparxList.push_back(s2);
}

void sparxUpdate() {
    for (auto& s : sparxList) {
        for (int i = 0; i < SPARX_SPEED; i++) {
            moveSparx(s);
        }
    }
}

bool sparxCollidesWithStix(int sx, int sy) {
    for (auto& s : sparxList) {
        int dx = abs((int)round(s.x) - sx);
        int dy = abs((int)round(s.y) - sy);
        if (dx <= 3 && dy <= 3) return true;
    }
    return false;
}

void sparxRender() {
    for (auto& s : sparxList) {
        drawCircle(s.x, s.y, 4, SPARX_COL_R, SPARX_COL_G, SPARX_COL_B, 8);
        drawCircle(s.x, s.y, 6, SPARX_COL_R, SPARX_COL_G * 0.8f, SPARX_COL_B * 0.3f, 8);
    }
}

void sparxAddNew(int x, int y, int dirX, int dirY) {
    SparxEntity s;
    s.x = x; s.y = y;
    s.dirX = dirX; s.dirY = dirY;
    s.speed = 1.0f;
    generateWishlist(s);
    sparxList.push_back(s);
}
