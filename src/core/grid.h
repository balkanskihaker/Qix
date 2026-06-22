#pragma once
#include <vector>

enum class CellState {
    FREE,
    BORDER,
    CLAIMED,
    WALL,
    TEMP,
    TEMP2
};

extern std::vector<std::vector<CellState>> grid;

void gridInit(int width, int height);
void gridRender();
CellState gridGet(int x, int y);
void gridSet(int x, int y, CellState state);
bool gridIsOnBorder(int x, int y);
int gridCountClaimed();
int gridTotal();
float gridClaimedPercent();
void floodFill(int startX, int startY, CellState fillState);