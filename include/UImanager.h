#pragma once
#include "raylib.h"
#include "player.h"
#include "TaskManager.h"
#include "scoremanager.h"

class UIManager {
private:

    Font pixelFont;
    Texture2D heartRed;
    Texture2D heartGrey;

    float introTimer;

public:
    UIManager();

    void Init();
    void DrawHUD(const Player& p, const TaskManager& tm, const ScoreManager& sm, float dt);

private:
    void DrawHearts(const Player& p);
    void DrawCurrentTask(const TaskManager& tm);
    void DrawElapsedTime(const ScoreManager& sm);

    void DrawIntroMessage();
    void DrawSpecialMessage(const TaskManager& tm);
};