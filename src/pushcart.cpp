#include "pushcart.h"

// Y POSITION NOTE:
// The position passed in (pos) should be set so the BOTTOM
// of the cart sits on the footpath.
// Recommended: pos.y = FOOTPATH_Y - 60  (60px = the full display height of the cart sprite)
// This way the cart wheels appear to rest ON the footpath.


PushCart::PushCart(Vector2 pos)
    : Obstacle(pos, 72.0f, 60.0f)
{
    texture = LoadTexture("assets/sprites/pushcart.png");
    SetTextureFilter(texture, TEXTURE_FILTER_POINT); // Keep pixel-art crisp at 3x
}


PushCart::~PushCart() {
    UnloadTexture(texture);
}


void PushCart::Update(float dt) {
    (void)dt; 
    // Nothing to update
}


void PushCart::Draw() {
    if (!isActive) return;

    DrawTextureEx(texture, position, 0.0f, 3.0f, WHITE);
}
//  Returns 1 as Player loses 1 heart when colliding with a pushcart.
//  Game::CheckCollisions() will call player->TakeDamage(obs)
//  which calls TakeDamage( obs.GetDamage() ) = TakeDamage(1).

int PushCart::GetDamage() {
    return 1;
}