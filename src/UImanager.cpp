#include "UImanager.h"

UIManager::UIManager() {
    introTimer = 3.0f;
}

void UIManager::Init() {
    heartRed  = LoadTexture("assets/hearts/redheart.png");
    heartGrey = LoadTexture("assets/hearts/greyheart.png");

    SetTextureFilter(heartRed, TEXTURE_FILTER_POINT);
    SetTextureFilter(heartGrey, TEXTURE_FILTER_POINT);
}

void UIManager::DrawHearts(const Player& p) {
    int x = 20;
    for (int i = 0; i < 5; i++) {
        Texture2D tex = (i < p.currentHearts) ? heartRed : heartGrey;
        DrawTextureEx(tex, {(float)x, 10}, 0, 3.0f, WHITE);
        x += 50;
    }
}

void UIManager::DrawCurrentTask(const TaskManager& tm) {
    if (tm.AllTasksDone()) {
        DrawText("All tasks done!", 20, 50, 20, GREEN);
        return;  // stop here, don't call GetCurrentTask()
    }
    std::string text = tm.GetCurrentTask().description;
    DrawText(text.c_str(), 20, 50, 20, BLACK);
}

void UIManager::DrawElapsedTime(const ScoreManager& sm) {
    DrawText(TextFormat("Time: %.1f", sm.GetElapsedTime()), 900, 10, 20, BLACK);
}

void UIManager::DrawIntroMessage() {
    const char* msg = "Traffic bohot hai, sambhal ke!";
    int w = MeasureText(msg, 24);
    DrawText(msg, (1152/2 - w/2), 20, 24, YELLOW);
}


void UIManager::DrawHUD(const Player& p, const TaskManager& tm, const ScoreManager& sm, float dt) {
    if (introTimer > 0) {
        introTimer -= dt;
        DrawIntroMessage();
    } else {
        DrawCurrentTask(tm);
        DrawSpecialMessage(tm);
    }

    DrawHearts(p);
    DrawElapsedTime(sm);
}


void UIManager::DrawSpecialMessage(const TaskManager& tm) {
    // temporary empty function
}