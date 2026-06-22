#pragma once
#include <vector>

struct Point {
    int x, y;
};

enum class DrawMode {
    NONE,
    SLOW,
    FAST
};

struct Stix {
    int x, y;
    DrawMode drawMode;
    bool isDrawing;
    std::vector<Point> trail;
    int primaryKey;
    bool keyPressed[4];
};

extern Stix stix;
extern int burnTimer;
extern int burnIndex;
extern bool isBurning;

void stixInit();
void stixUpdate();
void stixRender();
void stixHandleKey(int key);
void stixHandleKeyRelease(int key);
void stixHandleKeyChar(unsigned char key);