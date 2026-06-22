#include "qix.h"
#include "stix.h"
#include "../core/grid.h"
#include "../core/renderer.h"
#include "../utils/config.h"
#include <GLUT/glut.h>
#include <cmath>
#include <cstdlib>

Qix qix;

static float randomFloat(float min, float max) {
    return min + (float)rand() / RAND_MAX * (max - min);
}

void qixInit() {
    do {
        qix.x = BORDER_X + 1 + rand() % (BORDER_W - 2);
        qix.y = BORDER_Y + 1 + rand() % (BORDER_H - 2);
    } while (gridGet((int)qix.x, (int)qix.y) != CellState::FREE);

    qix.vx = randomFloat(-1.5f, 1.5f);
    qix.vy = randomFloat(-1.5f, 1.5f);

    if (fabs(qix.vx) < 0.5f) qix.vx = qix.vx < 0 ? -0.5f : 0.5f;
    if (fabs(qix.vy) < 0.5f) qix.vy = qix.vy < 0 ? -0.5f : 0.5f;

    qix.speed = QIX_SPEED;
    qix.tailLen = 0;
}

void qixUpdate() {
    if (qix.tailLen < 20) {
        qix.tailX[qix.tailLen] = qix.x;
        qix.tailY[qix.tailLen] = qix.y;
        qix.tailLen++;
    } else {
        for (int i = 0; i < 19; i++) {
            qix.tailX[i] = qix.tailX[i+1];
            qix.tailY[i] = qix.tailY[i+1];
        }
        qix.tailX[19] = qix.x;
        qix.tailY[19] = qix.y;
    }

    if (stix.isDrawing) {
        float dx = stix.x - qix.x;
        float dy = stix.y - qix.y;
        float dist = sqrtf(dx*dx + dy*dy);
        if (dist > 0) {
            qix.vx += (dx / dist) * 0.08f;
            qix.vy += (dy / dist) * 0.08f;
        }
    }

    if (rand() % 30 == 0) {
        qix.vx += randomFloat(-0.3f, 0.3f);
        qix.vy += randomFloat(-0.3f, 0.3f);
    }

    float speed = sqrtf(qix.vx*qix.vx + qix.vy*qix.vy);
    if (speed > qix.speed) {
        qix.vx = (qix.vx / speed) * qix.speed;
        qix.vy = (qix.vy / speed) * qix.speed;
    }
    if (speed < 0.5f) {
        qix.vx *= 1.5f;
        qix.vy *= 1.5f;
    }

    qix.x += qix.vx;
    qix.y += qix.vy;

    if (gridGet((int)qix.x, (int)qix.y) != CellState::FREE) {
        qix.x -= qix.vx * 2;
        qix.y -= qix.vy * 2;
        qix.vx *= -1;
        qix.vy *= -1;
    }

    if (qix.x <= BORDER_X + 1) { qix.x = BORDER_X + 1; qix.vx = fabs(qix.vx); }
    if (qix.x >= WINDOW_WIDTH - BORDER_X - 1) { qix.x = WINDOW_WIDTH - BORDER_X - 1; qix.vx = -fabs(qix.vx); }
    if (qix.y <= BORDER_Y + 1) { qix.y = BORDER_Y + 1; qix.vy = fabs(qix.vy); }
    if (qix.y >= WINDOW_HEIGHT - BORDER_Y - 1) { qix.y = WINDOW_HEIGHT - BORDER_Y - 1; qix.vy = -fabs(qix.vy); }
}

bool qixCollidesWithTrail() {
    if (!stix.isDrawing) return false;
    int qx = (int)round(qix.x);
    int qy = (int)round(qix.y);
    for (auto& p : stix.trail) {
        if (abs(qx - p.x) <= 2 && abs(qy - p.y) <= 2) return true;
    }
    return false;
}

void qixRender() {
    // Tail
    for (int i = 0; i < qix.tailLen - 1; i++) {
        float alpha = (float)i / qix.tailLen;
        drawLine(qix.tailX[i], qix.tailY[i],
                 qix.tailX[i+1], qix.tailY[i+1],
                 QIX_TAIL_R * alpha, QIX_TAIL_G * alpha, QIX_TAIL_B * alpha, 1.5f);
    }

    // Telo — animirani oblik
    float t = glutGet(GLUT_ELAPSED_TIME) * 0.003f;
    for (int i = 0; i < 6; i++) {
        float angle  = (i / 6.0f) * 3.14159f * 2 + t;
        float r      = 8.0f + sinf(t * 3 + i) * 3.0f;
        float x1     = qix.x + cosf(angle) * r;
        float y1     = qix.y + sinf(angle) * r;
        float angle2 = ((i+1) / 6.0f) * 3.14159f * 2 + t;
        float x2     = qix.x + cosf(angle2) * r;
        float y2     = qix.y + sinf(angle2) * r;
        drawLine(x1, y1, x2, y2, QIX_COL_R, QIX_COL_G, QIX_COL_B, 2.0f);
    }
}
