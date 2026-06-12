#include "map.h"
#include "raymath.h"

void Map::Init() {
    bg = LoadTexture("assets/backgrounds/bg.png");
    sky = LoadTexture("assets/backgrounds/sky.png");
    SetTextureFilter(bg, TEXTURE_FILTER_POINT);
    SetTextureFilter(sky, TEXTURE_FILTER_POINT);
    skyOffsetX = 0;
    prevCamX = 0;
}

void Map::Update(float camX)
{
    float camDelta = camX - prevCamX;
    prevCamX = camX;

    const float baseSpeed = 15.0f;

    // Sky always drifts left
    float speed = baseSpeed + camDelta * 0.10f;

    // Prevent sky from stopping or reversing
    speed = Clamp(speed, 5.0f, 40.0f);

    skyOffsetX += speed * GetFrameTime();

    float tileW = sky.width * 3.0f;

    while (skyOffsetX >= tileW)
        skyOffsetX -= tileW;

    while (skyOffsetX < 0)
        skyOffsetX += tileW;
}

void Map::Draw(Camera2D camera)
{
    float scale = 3.0f;

    float tileW = sky.width * scale;
    float startX = -skyOffsetX;

    // Infinite sky
    for (int i = -1; i <= 2; i++)
    {
        DrawTextureEx(
            sky,
            { startX + i * tileW, 0 },
            0,
            scale,
            WHITE
        );
    }

    // Main Karachi background
    DrawTextureEx(
        bg,
        {0, 0},
        0,
        scale,
        WHITE
    );
}

void Map::Unload() {
    UnloadTexture(bg);
    UnloadTexture(sky);
}