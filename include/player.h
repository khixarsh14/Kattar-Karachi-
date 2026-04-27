#pragma once
#include "raylib.h"
#include "main.h"

// forward declare so we don't need to include the full headers here
class UIManager;
class Obstacle;

class Player {
private:
    Vector2 position;
    float velX;
    float velY;

    bool isOnGround;
    bool facingRight;
    bool isJumping;
    bool isMoving;

    // hearts + damage
    int currentHearts;
    int maxHearts;
    bool isInvincible;
    float invincTimer;
    float blinkTimer;
    bool blinkVisible;

    // sprites
    Texture2D texIdle;
    Texture2D texWalk;
    Texture2D texJump;

    // walk animation
    int currentFrame;
    float frameTimer;
    static const int WALK_FRAMES = 8;
    static constexpr float FRAME_SPEED = 0.08f;

    float groundY;

    friend class UIManager;

public:
    Player();
    ~Player();

    void Init(Vector2 startPos);
    void Update(float dt);
    void Draw();

    void TakeDamage(int amount);
    void TakeDamage(Obstacle& obs);

    bool IsAlive()       const;
    bool IsFacingRight() const;
    bool IsOnGround()    const;
    Vector2   GetPosition() const;
    Rectangle GetBounds()   const;

    float GetVelocityY() const;
    void SetPositionY(float y);
    void SetPositionX(float x);
    void SetOnGround(bool val);
};