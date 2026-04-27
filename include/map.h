#pragma once
#include "raylib.h"

class Map {
private:
    Texture2D bg;

public:
    void Init();
    void Draw();
    void Unload();
};