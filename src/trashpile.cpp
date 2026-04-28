#include "TrashPile.h"

TrashPile::TrashPile(Vector2 pos)
    : Obstacle(pos, 38.0f, 22.0f,2)  // base size
{
    texture = LoadTexture("assets/obstacles/trash.png");
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
}

TrashPile::~TrashPile() {
    UnloadTexture(texture);
}

void TrashPile::Update(float dt) {
    (void)dt;
}

void TrashPile::Draw() {
    if (!isActive) return;
    DrawTextureEx(texture, position, 0.0f, 3.0f, WHITE);
}

int TrashPile::GetDamage() {
    return 1;
}

Rectangle TrashPile::GetBounds() {
    float scale = 3.0f;
    float w = width * scale;   // 114px
    float h = height * scale;  // 66px
    
    // standardized hitbox - focus on lower portion where collision matters
    return {
        position.x + 23,
        position.y + 15,   // 🔼 moved UP (was 35)
        w - 35,
        h - 25             // slightly taller too (was -35)
    };
}