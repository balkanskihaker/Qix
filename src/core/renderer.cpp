#include "renderer.h"
#include "../utils/config.h"
#include <GLUT/glut.h>
#include <cmath>
#include "RgbImage.h"

static GLuint crtTexture = 0;

void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void drawRectOutline(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void drawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float thickness) {
    glColor3f(r, g, b);
    glLineWidth(thickness);
    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
    glEnd();
    glLineWidth(1.0f);
}

void drawCircle(float x, float y, float radius, float r, float g, float b, int segments) {
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= segments; i++) {
            float angle = 2.0f * 3.14159f * i / segments;
            glVertex2f(x + radius * cosf(angle), y + radius * sinf(angle));
        }
    glEnd();
}

void drawText(float x, float y, const char* text, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *text);
        text++;
    }
}

void initCRTTexture() {
    RgbImage img("assets/crt_overlay.bmp");
    printf("CRT texture loaded: %d, size: %ld x %ld\n", 
           img.ImageLoaded(), img.GetNumCols(), img.GetNumRows());
    if (!img.ImageLoaded()) {
        fprintf(stderr, "Failed to load CRT overlay texture\n");
        return;
    }

    glGenTextures(1, &crtTexture);
    glBindTexture(GL_TEXTURE_2D, crtTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,
                      img.GetNumCols(), img.GetNumRows(),
                      GL_RGB, GL_UNSIGNED_BYTE, img.ImageData());
}

void drawCRTEffect() {
    if (crtTexture == 0) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_COLOR, GL_ZERO);  // Multiply blend — tamni pikseli u teksturi tame ekran
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, crtTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(0, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(WINDOW_WIDTH, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(0, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}