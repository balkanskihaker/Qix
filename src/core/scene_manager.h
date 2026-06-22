#pragma once

enum class Scene {
    MENU,
    GAME,
    SCOREBOARD
};

extern Scene currentScene;

void sceneInit(Scene scene);
void sceneUpdate(Scene scene);
void sceneRender(Scene scene);
void switchScene(Scene scene);