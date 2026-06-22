#include "grid.h"
#include "renderer.h"
#include "../utils/config.h"
#include <GLUT/glut.h>
#include <stack>

std::vector<std::vector<CellState>> grid;

void gridInit(int width, int height) {
    grid.assign(height, std::vector<CellState>(width, CellState::FREE));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < BORDER_X || x > width - BORDER_X - 1 ||
                y < BORDER_Y || y > height - BORDER_Y - 1) {
                grid[y][x] = CellState::BORDER;
            }
        }
    }

    for (int x = BORDER_X; x <= width - BORDER_X; x++)
        grid[BORDER_Y][x] = CellState::BORDER;
    for (int x = BORDER_X; x <= width - BORDER_X; x++)
        grid[height - BORDER_Y - 1][x] = CellState::BORDER;
    for (int y = BORDER_Y; y <= height - BORDER_Y - 1; y++)
        grid[y][BORDER_X] = CellState::BORDER;
    for (int y = BORDER_Y; y <= height - BORDER_Y - 1; y++)
        grid[y][width - BORDER_X - 1] = CellState::BORDER;
}

void gridRender() {
    // Border i WALL — plava
    glColor3f(BORDER_COL_R, BORDER_COL_G, BORDER_COL_B);
    glBegin(GL_QUADS);
    for (int y = BORDER_Y; y < WINDOW_HEIGHT - BORDER_Y; y++) {
        for (int x = BORDER_X; x < WINDOW_WIDTH - BORDER_X; x++) {
            if (grid[y][x] == CellState::BORDER || grid[y][x] == CellState::WALL) {
                glVertex2f(x,     y);
                glVertex2f(x + 1, y);
                glVertex2f(x + 1, y + 1);
                glVertex2f(x,     y + 1);
            }
        }
    }
    glEnd();

    // Claimed teritorija — tamno ljubicasta
    glColor3f(CLAIMED_COL_R, CLAIMED_COL_G, CLAIMED_COL_B);
    glBegin(GL_QUADS);
    for (int y = BORDER_Y; y < WINDOW_HEIGHT - BORDER_Y; y++) {
        for (int x = BORDER_X; x < WINDOW_WIDTH - BORDER_X; x++) {
            if (grid[y][x] == CellState::CLAIMED) {
                glVertex2f(x,     y);
                glVertex2f(x + 1, y);
                glVertex2f(x + 1, y + 1);
                glVertex2f(x,     y + 1);
            }
        }
    }
    glEnd();
}

CellState gridGet(int x, int y) {
    if (x < 0 || y < 0 || x >= (int)grid[0].size() || y >= (int)grid.size())
        return CellState::BORDER;
    return grid[y][x];
}

void gridSet(int x, int y, CellState state) {
    if (x < 0 || y < 0 || x >= (int)grid[0].size() || y >= (int)grid.size())
        return;
    grid[y][x] = state;
}

bool gridIsOnBorder(int x, int y) {
    CellState s = gridGet(x, y);
    return s == CellState::BORDER || s == CellState::WALL;
}

int gridCountClaimed() {
    int count = 0;
    for (auto& row : grid)
        for (auto& cell : row)
            if (cell == CellState::CLAIMED || cell == CellState::BORDER || cell == CellState::WALL)
                count++;
    return count;
}

int gridTotal() {
    return BORDER_W * BORDER_H;
}

float gridClaimedPercent() {
    int claimed = 0;
    int total = 0;
    for (int y = BORDER_Y; y < WINDOW_HEIGHT - BORDER_Y; y++) {
        for (int x = BORDER_X; x < WINDOW_WIDTH - BORDER_X; x++) {
            total++;
            if (grid[y][x] == CellState::CLAIMED || grid[y][x] == CellState::WALL)
                claimed++;
        }
    }
    return (float)claimed / (float)total * 100.0f;
}

void floodFill(int startX, int startY, CellState fillState) {
    if (gridGet(startX, startY) != CellState::FREE) return;

    std::stack<std::pair<int,int>> stack;
    stack.push({startX, startY});

    while (!stack.empty()) {
        auto [x, y] = stack.top();
        stack.pop();

        if (gridGet(x, y) != CellState::FREE) continue;

        gridSet(x, y, fillState);

        stack.push({x + 1, y});
        stack.push({x - 1, y});
        stack.push({x, y + 1});
        stack.push({x, y - 1});
    }
}
