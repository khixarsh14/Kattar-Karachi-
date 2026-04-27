#include "map.h"

void Map::Init() {
    bg = LoadTexture("assets/backgrounds/bg.png");
    SetTextureFilter(bg, TEXTURE_FILTER_POINT);
}

void Map::Draw() {
    // draw scaled 3x → 1024 * 3 = 3072 width
    DrawTextureEx(bg, {0, 0}, 0.0f, 3.0f, WHITE);
}

void Map::Unload() {
    UnloadTexture(bg);
}