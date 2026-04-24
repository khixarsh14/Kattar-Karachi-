#include "obstacle.h"


Obstacle::Obstacle(Vector2 pos, float w, float h)
    : position(pos), width(w), height(h), isActive(true)
{}


Obstacle::~Obstacle() {
}


//  Returns false for all obstacles by default.
//  Only Pothole overrides this to return true.
//  Game checks IsLethal() first â€” if true, it triggers instant game over without checking hearts. instant death function
bool Obstacle::IsLethal() {
    return false;
}


Rectangle Obstacle::GetBounds() {
    return { position.x, position.y, width, height };
}

//  Returns whether this obstacle should be checked for collisions and drawn on screen.
bool Obstacle::IsActive() const {
    return isActive;
}


//  Enable or disable this obstacle.
//  Game can use this to deactivate an obstacle after it has

void Obstacle::SetActive(bool active) {
    isActive = active;
}

//  Game uses this to check if an obstacle is near the player  before running a full rectangle collision check.

Vector2 Obstacle::GetPosition() const {
    return position;
}