#pragma once
#include "raylib.h"

inline bool IsLandingOnTop(Rectangle player, Rectangle platform, float velY)
{
    if (velY <= 0) return false;
    
    float playerBottom = player.y + player.height;
    float platformTop = platform.y;
    return (playerBottom >= platformTop && playerBottom <= platformTop + 15);
}

enum GameState {
    STATE_START,
    STATE_PLAYING,
    STATE_WON,
    STATE_GAMEOVER
};

class Game {
public:
    void Run();
};