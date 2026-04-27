#include "Pothole.h"

Pothole::Pothole(Vector2 pos)
    : Obstacle(pos, 30.0f, 10.0f)  // base size
{
    texture = LoadTexture("assets/obstacles/pothole.png");
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
}

Pothole::~Pothole() {
    UnloadTexture(texture);
}

void Pothole::Update(float dt) {
    (void)dt;
}

void Pothole::Draw() {
    if (!isActive) return;
    DrawTextureEx(texture, position, 0.0f, 3.0f, WHITE);
}

int Pothole::GetDamage() {
    return 999;
}

bool Pothole::IsLethal() {
    return true;
}

Rectangle Pothole::GetBounds() {
    float scale = 3.0f;
    float w = width * scale;   // 90px
    float h = height * scale;  // 30px
    
    // standardized hitbox - slightly smaller than sprite
    return {
        position.x + 24,
        position.y + 5,
        w - 50,
        h - 15
    };
}