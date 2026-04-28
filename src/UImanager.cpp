#include "UImanager.h"

UIManager::UIManager() {
    introTimer = 6.0f;
}

void UIManager::Init() {
    heartRed  = LoadTexture("assets/hearts/redheart.png");
    heartGrey = LoadTexture("assets/hearts/greyheart.png");

    SetTextureFilter(heartRed, TEXTURE_FILTER_POINT);
    SetTextureFilter(heartGrey, TEXTURE_FILTER_POINT);
    pixelFont = LoadFontEx("assets/font/PressStart2P-Regular.ttf", 22, nullptr, 0);
    SetTextureFilter(pixelFont.texture, TEXTURE_FILTER_POINT);
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
        DrawTextEx(pixelFont, "All tasks done!", {22, 76}, 18, 2, BLACK);
        DrawTextEx(pixelFont, "All tasks done!", {20, 74}, 18, 2, GREEN);
        return; // stop here, don't call GetCurrentTask()
    }
    std::string text = tm.GetCurrentTask().description;
    DrawTextEx(pixelFont, text.c_str(), {22, 76}, 18, 2, BLACK);
    DrawTextEx(pixelFont, text.c_str(), {20, 74}, 18, 2, WHITE);
}

void UIManager::DrawElapsedTime(const ScoreManager& sm) {
    const char* timeText = TextFormat("Time: %.1f", sm.GetElapsedTime());
    DrawTextEx(pixelFont, timeText, {902, 12}, 18, 2, BLACK);
    DrawTextEx(pixelFont, timeText, {900, 10}, 18, 2, WHITE);
}

void UIManager::DrawIntroMessage() {
    const char* msg = "Traffic bohot hai, sambhal ke!";
    Vector2 size = MeasureTextEx(pixelFont, msg, 25, 2);
    float x = (1152 / 2) - (size.x / 2);
    // shadow then main text
    DrawTextEx(pixelFont, msg, {x + 2, 76}, 25, 2, BLACK);
    DrawTextEx(pixelFont, msg, {x,     74}, 25, 2, YELLOW);
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