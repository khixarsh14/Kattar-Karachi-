#include "Pothole.h"



Pothole::Pothole(Vector2 pos)
    : Obstacle(pos, 90.0f, 36.0f)
{
    texture = LoadTexture("assets/sprites/pothole.png");
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