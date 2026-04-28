#pragma once
#include "raylib.h"

class Map {
private:
    Texture2D bg;
    Texture2D sky;      // ⭐ NEW: Sky layer
    float skyOffsetX;   // ⭐ Parallax offset

public:
    void Init();
    void Draw(Camera2D camera);  // ⭐ Pass camera
    void Update(float camX);     // ⭐ Update parallax
    void Unload();
};