#include "pushcart.h"

PushCart::PushCart(Vector2 pos)
    : Obstacle(pos, 45.0f, 19.0f)  // base size
{
    texture = LoadTexture("assets/obstacles/pushcart.png");
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
}

PushCart::~PushCart() {
    UnloadTexture(texture);
}

void PushCart::Update(float dt) {
    (void)dt;
}

void PushCart::Draw() {
    if (!isActive) return;
    DrawTextureEx(texture, position, 0.0f, 3.0f, WHITE);
}

int PushCart::GetDamage() {
    return 1;
}

// landing surface at y=3 base pixels from top (user specified)
// position.y + (3 * 3) = position.y + 9 display pixels
Rectangle PushCart::GetBounds() {
    float scale = 3.0f;
    float w = width * scale;   // 135px
    float h = height * scale;  // 57px
    
    return {
        position.x + 8,       // slight horizontal padding
        position.y + 6,      
        w - 20,                // 115px wide hitbox
        h - 9                  // 48px tall hitbox
    };
}