#include "trashpile.h"


TrashPile::TrashPile(Vector2 pos)
    : Obstacle(pos, 60.0f, 48.0f)
{
    texture = LoadTexture("assets/sprites/trashpile.png");
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