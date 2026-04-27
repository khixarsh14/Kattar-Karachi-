#include "obstacle.h"

Obstacle::Obstacle(Vector2 pos, float w, float h)
    : position(pos), width(w), height(h), isActive(true)
{}

Obstacle::~Obstacle() {
    UnloadTexture(texture);
}

bool Obstacle::IsLethal() {
    return false;  // default: not lethal, Pothole overrides
}

// default GetBounds - subclasses should override for custom hitboxes
Rectangle Obstacle::GetBounds() {
    float scale = 3.0f;
    return {
        position.x,
        position.y,
        width * scale,
        height * scale
    };
}

bool Obstacle::IsActive() const {
    return isActive;
}

void Obstacle::SetActive(bool active) {
    isActive = active;
}

Vector2 Obstacle::GetPosition() const {
    return position;
}