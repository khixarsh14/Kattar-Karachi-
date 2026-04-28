#pragma once
#include "raylib.h"

class Shopper {
private:
    Vector2 basePos;
    Texture2D texture;

    float time;
    float amplitude;
    float speed;

    bool active;
    float currentOffsetY;   // NEW

public:
    void Init(const char* path);
    void Spawn(float centerX, float groundY);
    void Update(float dt);
    void Draw();
    void Deactivate();

    bool IsActive() const;
    Rectangle GetBounds() const;
};