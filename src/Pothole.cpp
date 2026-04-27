#include "Pothole.h"



Pothole::Pothole(Vector2 pos)
    : Obstacle(pos, 90.0f, 36.0f)
{
    texture = LoadTexture("assets/obstacles/pothole.png");
    SetTextureFilter(texture, TEXTURE_FILTER_POINT); // Keep pixel-art crisp at 3x
}

Pothole::~Pothole() {
    UnloadTexture(texture);
}


//  Pothole is completely static it never moves.
//  dt (delta time) is ignored.
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

    float w = 90.0f * scale;
    float h = 36.0f * scale;

    return {
        position.x + 20,   // shrink from left
        position.y + 15,   // push down (important)
        w - 40,            // narrower
        h - 20             // shorter
    };
}