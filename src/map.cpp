#include "map.h"

void Map::Init() {
    bg = LoadTexture("assets/backgrounds/bg.png");
    sky = LoadTexture("assets/backgrounds/sky.png");
    SetTextureFilter(bg, TEXTURE_FILTER_POINT);
    SetTextureFilter(sky, TEXTURE_FILTER_POINT);
    skyOffsetX = 0;
}

void Map::Update(float camX) {
    skyOffsetX = camX * 0.3f; 
}

void Map::Draw(Camera2D camera) {
   
    DrawTextureEx(sky, {-skyOffsetX, 0}, 0.0f, 3.0f, WHITE);
    DrawTextureEx(sky, {-skyOffsetX + sky.width * 3.0f, 0}, 0.0f, 3.0f, WHITE); 
    // Map layer (normal speed)
    DrawTextureEx(bg, {0, 0}, 0.0f, 3.0f, WHITE);
}

void Map::Unload() {
    UnloadTexture(bg);
    UnloadTexture(sky);
}