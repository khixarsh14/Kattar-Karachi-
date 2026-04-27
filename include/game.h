#ifndef GAME_H
#define GAME_H

#include "raylib.h"

// helper for platform collision detection
inline bool IsLandingOnTop(Rectangle player, Rectangle platform, float velY)
{
    // player must be falling (positive Y velocity)
    if (velY <= 0) return false;
    
    // player's bottom must be near platform's top
    float playerBottom = player.y + player.height;
    float platformTop = platform.y;
    
    // within 15px tolerance + player coming from above
    return (playerBottom >= platformTop && playerBottom <= platformTop + 15);
}

class Game {
public:
    void Run();
};

#endif