#include "Shopper.h"
#include <cmath>

void Shopper::Init(const char* path) {
    texture = LoadTexture(path);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    time = 0.0f;
    amplitude = 10.0f;   // bounce height
    speed = 4.0f;        // bounce speed
    active = false;
    currentOffsetY = 0.0f;
}

void Shopper::Spawn(float centerX, float groundY) {
    float scale = 3.0f;

    float width  = texture.width * scale;
    float height = texture.height * scale;

    // center align
    basePos.x = centerX - width / 2.0f;
    basePos.y = groundY - 10;

    time = 0.0f;
    active = true;
}

void Shopper::Update(float dt) {
    if (!active) return;
    time += dt;
    currentOffsetY = sinf(time * speed) * amplitude;
}

void Shopper::Draw() {
    if (!active) return;

    float offsetY = sinf(time * speed) * amplitude;

    DrawTextureEx(texture,
        { basePos.x, basePos.y + currentOffsetY },
        0.0f,
        3.0f,
        WHITE
    );
}

void Shopper::Deactivate() {
    active = false;
}

bool Shopper::IsActive() const {
    return active;
}



Rectangle Shopper::GetBounds() const {
    float scale = 3.0f;

    return {
        basePos.x + 5,
        basePos.y + currentOffsetY + 5,
        texture.width * scale - 10,
        texture.height * scale - 10
    };
}