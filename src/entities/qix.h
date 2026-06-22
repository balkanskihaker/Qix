#pragma once

struct Qix {
    float x, y;
    float vx, vy;
    float speed;
    float tailX[20];
    float tailY[20];
    int tailLen;
};

extern Qix qix;

void qixInit();
void qixUpdate();
void qixRender();
bool qixCollidesWithTrail();