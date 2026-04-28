#ifndef GAME_H
#define GAME_H

#include "raylib.h"

// helper for platform collision detection
inline bool IsLandingOnTop(Rectangle player, Rectangle platform, float velY)
{
    if (velY <= 0) return false;
    float playerBottom = player.y + player.height;
    float platformTop  = platform.y;
    return (playerBottom >= platformTop && playerBottom <= platformTop + 15);
}

enum GameState {
    STATE_START,
    STATE_PLAYING,
    STATE_GAMEOVER
};

class Game {
public:
    void Run();
};

#endif