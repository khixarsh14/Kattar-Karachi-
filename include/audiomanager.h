#pragma once
#include "raylib.h"

class AudioManager {
private:
    // Music
    Music bgMusic;

    // Sounds
    Sound sndPothole;
    Sound sndPushcart;
    Sound sndTrashpile;
    Sound sndTaskComplete;
    Sound sndGameOver;
    Sound sndButton;

    // Flags to prevent repeated triggering
    bool gameOverPlayed;

public:
    AudioManager();
    ~AudioManager();

    void Init();
    void Update();
    void Unload();

    // Playback triggers (called from game.cpp at the right moments)
    void PlayObstacleSound(int obstacleType); // 0=pothole, 1=pushcart, 2=trashpile
    void PlayTaskComplete();
    void PlayGameOver();
};