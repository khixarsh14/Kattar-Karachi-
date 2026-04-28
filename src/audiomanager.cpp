#include "audiomanager.h"

// obstacleType constants for clarity
// 0 = Pothole
// 1 = PushCart
// 2 = TrashPile

AudioManager::AudioManager()
    : gameOverPlayed(false)
{}

AudioManager::~AudioManager() {}

void AudioManager::Init() {

    bgMusic       = LoadMusicStream("assets/sounds/backgroundmusic.mp3"); 
    sndPushcart   = LoadSound("assets/sounds/pushcart.mp3");
    sndPothole      = LoadSound("assets/sounds/pothole.mp3");
    sndTrashpile  = LoadSound("assets/sounds/trashpile.mp3");
    sndTaskComplete = LoadSound("assets/sounds/taskcompletion.mp3"); 
    sndGameOver   = LoadSound("assets/sounds/gameover.mp3"); 

    SetMusicVolume(bgMusic, 0.5f);
    PlayMusicStream(bgMusic);
}

void AudioManager::Update() {
    UpdateMusicStream(bgMusic);   // MUST be called every frame or music stops
}

void AudioManager::PlayObstacleSound(int obstacleType) {
    if (obstacleType == 0) PlaySound(sndPothole);
    else if (obstacleType == 1) PlaySound(sndPushcart);
    else if (obstacleType == 2) PlaySound(sndTrashpile);
}

void AudioManager::PlayTaskComplete() {
    PlaySound(sndTaskComplete);
}

void AudioManager::PlayGameOver() {
    if (!gameOverPlayed) {
        StopMusicStream(bgMusic);
        PlaySound(sndGameOver);
        gameOverPlayed = true;
    }
}

void AudioManager::Unload() {
    UnloadMusicStream(bgMusic);
    UnloadSound(sndPothole);
    UnloadSound(sndPushcart);
    UnloadSound(sndTrashpile);
    UnloadSound(sndTaskComplete);
    UnloadSound(sndGameOver);
}
