#pragma once
#include <vector>
#include "../core/grid.h"

struct SparxEntity {
    float x, y;
    int dirX, dirY;
    float speed;
    int wishlist[3][2];
};

extern std::vector<SparxEntity> sparxList;

void sparxInit();
void sparxUpdate();
void sparxRender();
bool sparxCollidesWithStix(int sx, int sy);
void sparxAddNew(int x, int y, int dirX, int dirY);