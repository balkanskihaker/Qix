#pragma once

void drawRect(float x, float y, float width, float height, float r, float g, float b);
void drawRectOutline(float x, float y, float width, float height, float r, float g, float b);
void drawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float thickness);
void drawCircle(float x, float y, float radius, float r, float g, float b, int segments);
void drawText(float x, float y, const char* text, float r, float g, float b);
void drawCRTEffect();
void initCRTTexture();
void drawCRTEffect();