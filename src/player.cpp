#include "player.h"
#include "obstacle.h"
#include "raymath.h"

Player::Player() {
    position    = {0, 0};
    velX        = 0;
    velY        = 0;
    isOnGround  = true;
    facingRight = true;
    isJumping   = false;
    isMoving    = false;
    groundY     = 350.0f;

    currentHearts = MAX_HEARTS;
    maxHearts     = MAX_HEARTS;
    isInvincible  = false;
    invincTimer   = 0;
    blinkTimer    = 0;
    blinkVisible  = true;

    currentFrame = 0;
    frameTimer   = 0;
}

Player::~Player() {
    UnloadTexture(texIdle);
    UnloadTexture(texWalk);
    UnloadTexture(texJump);
}

void Player::Init(Vector2 startPos) {
    position = startPos;
    groundY  = startPos.y;

    texIdle = LoadTexture("assets/sprites/player_idle.png");
    texWalk = LoadTexture("assets/sprites/player_walk.png");
    texJump = LoadTexture("assets/sprites/player_jump.png");

    SetTextureFilter(texIdle, TEXTURE_FILTER_POINT);
    SetTextureFilter(texWalk, TEXTURE_FILTER_POINT);
    SetTextureFilter(texJump, TEXTURE_FILTER_POINT);
}

void Player::Update(float dt) {
    isMoving = false;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        velX = PLAYER_SPEED;
        facingRight = true;
        isMoving = true;
    } else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        velX = -PLAYER_SPEED;
        facingRight = false;
        isMoving = true;
    } else {
        velX = 0;
    }

    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && isOnGround) {
        velY = PLAYER_JUMP;
        isOnGround = false;
        isJumping  = true;
    }

    if ((IsKeyReleased(KEY_SPACE) || IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && velY < 0) {
        velY *= 0.45f;
    }

    if (!isOnGround) {
        velY += GRAVITY * dt;
    }

    position.x += velX * dt;
    position.y += velY * dt;

    if (position.y >= groundY) {
        position.y = groundY;
        velY       = 0;
        isOnGround = true;
        isJumping  = false;
    }

    if (isInvincible) {
        invincTimer -= dt;
        blinkTimer  += dt;
        if (blinkTimer >= 0.1f) {
            blinkVisible = !blinkVisible;
            blinkTimer   = 0;
        }
        if (invincTimer <= 0) {
            isInvincible = false;
            blinkVisible = true;
        }
    }

    if (isOnGround && isMoving) {
        frameTimer += dt;
        if (frameTimer >= FRAME_SPEED) {
            currentFrame = (currentFrame + 1) % WALK_FRAMES;
            frameTimer   = 0;
        }
    } else if (isOnGround && !isMoving) {
        currentFrame = 0;
        frameTimer   = 0;
    }


}

void Player::Draw() {
    if (!blinkVisible) return;

    const int BASE_W  = 20;
    const int BASE_H  = 30;
    const int DISP_W  = BASE_W * SCALE;
    const int DISP_H  = BASE_H * SCALE;

    // sprite draws with BOTTOM at position.y
    Rectangle dest = { position.x, position.y - DISP_H, (float)DISP_W, (float)DISP_H };
    
    if (isJumping && !isOnGround) {
        Rectangle src = {
            0, 0,
            facingRight ? (float)BASE_W : -(float)BASE_W,
            (float)BASE_H
        };
        DrawTexturePro(texJump, src, dest, {0, 0}, 0, WHITE);
    } else if (isMoving) {
        Rectangle src = {
            (float)(currentFrame * BASE_W),
            0,
            facingRight ? (float)BASE_W : -(float)BASE_W,
            (float)BASE_H
        };
        DrawTexturePro(texWalk, src, dest, {0, 0}, 0, WHITE);
    } else {
        Rectangle src = {
            0, 0,
            facingRight ? (float)BASE_W : -(float)BASE_W,
            (float)BASE_H
        };
        DrawTexturePro(texIdle, src, dest, {0, 0}, 0, WHITE);
    }
}

void Player::TakeDamage(int amount) {
    if (isInvincible) return;

    currentHearts -= amount;
    if (currentHearts < 0) currentHearts = 0;

    isInvincible = true;
    invincTimer  = INVINCIBLE_TIME;
    blinkTimer   = 0;
    blinkVisible = true;
}

void Player::TakeDamage(Obstacle& obs) {
    TakeDamage(obs.GetDamage());
}

bool Player::IsAlive() const {
    return currentHearts > 0;
}

bool Player::IsFacingRight() const {
    return facingRight;
}

bool Player::IsOnGround() const {
    return isOnGround;
}

Vector2 Player::GetPosition() const {
    return position;
}

// CRITICAL FIX: hitbox MUST match where sprite is drawn
Rectangle Player::GetBounds() const {
    const int DISP_H = 30 * SCALE;  // 90px
    
    // sprite bottom = position.y
    // sprite top = position.y - 90
    // hitbox slightly smaller for fair gameplay
    return Rectangle{
        position.x + 12,           // 12px padding on sides
        position.y - DISP_H + 20,  // 20px from top of sprite
        36,                         // 36px wide (vs 60px sprite)
        65                          // 65px tall (vs 90px sprite)
    };
}

float Player::GetVelocityY() const { 
    return velY; 
}

void Player::SetPositionY(float y) {
    position.y = y;
    velY = 0;
    isJumping = false; 
}

void Player::SetPositionX(float x) {
    position.x = x;
}

void Player::SetOnGround(bool val) {
    isOnGround = val;
}

