#pragma once
#include "raylib.h"

class Map {
private:
    Texture2D bg;
    Texture2D sky;
    float skyOffsetX;

public:
    void Init();
    void Draw(Camera2D camera);  
    void Update(float camX);   
    void Unload();
};