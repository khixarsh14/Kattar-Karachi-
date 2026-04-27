#include "TrashPile.h"


TrashPile::TrashPile(Vector2 pos)
    : Obstacle(pos, 60.0f, 48.0f)
{
    texture = LoadTexture("assets/obstacles/trash.png");
    SetTextureFilter(texture, TEXTURE_FILTER_POINT); // Keep pixel-art crisp at 3x
}


TrashPile::~TrashPile() {
    UnloadTexture(texture);
}

void TrashPile::Update(float dt) {
    (void)dt; 
    // Nothing to update
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
    float w = 60.0f * scale;
    float h = 48.0f * scale;

    return {
        position.x + 20,
        position.y + 30,   // lower hitbox
        w - 40,
        h - 35
    };
}