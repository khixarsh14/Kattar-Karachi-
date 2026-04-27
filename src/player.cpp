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
    groundY  = startPos.y;  // wherever we spawn is the floor

    texIdle = LoadTexture("assets/sprites/player_idle.png");
    texWalk = LoadTexture("assets/sprites/player_walk.png");
    texJump = LoadTexture("assets/sprites/player_jump.png");

    // pixel art - no blurring
    SetTextureFilter(texIdle, TEXTURE_FILTER_POINT);
    SetTextureFilter(texWalk, TEXTURE_FILTER_POINT);
    SetTextureFilter(texJump, TEXTURE_FILTER_POINT);
}

void Player::Update(float dt) {
    isMoving = false;

    // --- horizontal movement, works in air too ---
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

    // --- jump start ---
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && isOnGround) {
        velY = PLAYER_JUMP;   // negative = up in raylib
        isOnGround = false;
        isJumping  = true;
    }

    // --- jump cut: let go early = shorter jump (feels way better) ---
    if ((IsKeyReleased(KEY_SPACE) || IsKeyReleased(KEY_UP) || IsKeyReleased(KEY_W)) && velY < 0) {
        velY *= 0.45f;
    }

    // --- gravity ---
    if (!isOnGround) {
        velY += GRAVITY * dt;
    }

    // --- apply velocity ---
    position.x += velX * dt;
    position.y += velY * dt;

    // --- land ---
    if (position.y >= groundY) {
        position.y = groundY;
        velY       = 0;
        isOnGround = true;
        isJumping  = false;
    }



    // --- invincibility blink ---
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

    // --- walk animation (only on ground, only moving) ---
    if (isOnGround && isMoving) {
        frameTimer += dt;
        if (frameTimer >= FRAME_SPEED) {
            currentFrame = (currentFrame + 1) % WALK_FRAMES;
            frameTimer   = 0;
        }
    } else if (isOnGround && !isMoving) {
        // reset to first frame when standing still
        currentFrame = 0;
        frameTimer   = 0;
    }
}

void Player::Draw() {
    if (!blinkVisible) return;  // skip frame when blinking = damage effect

    const int BASE_W  = 20;
    const int BASE_H  = 30;
    const int DISP_W  = BASE_W * SCALE;   // 60px on screen
    const int DISP_H  = BASE_H * SCALE;   // 90px on screen

    Rectangle dest = { position.x, position.y - DISP_H, (float)DISP_W, (float)DISP_H };
    if (isJumping) {
        // jump sprite - single frame, flip by making src width negative
        Rectangle src = {
            0, 0,
            facingRight ? (float)BASE_W : -(float)BASE_W,
            (float)BASE_H
        };
        DrawTexturePro(texJump, src, dest, {0, 0}, 0, WHITE);

    } else if (isMoving) {
        // walk spritesheet - 8 frames side by side, each 20px wide
        Rectangle src = {
            (float)(currentFrame * BASE_W),
            0,
            facingRight ? (float)BASE_W : -(float)BASE_W,
            (float)BASE_H
        };
        DrawTexturePro(texWalk, src, dest, {0, 0}, 0, WHITE);

    } else {
        // idle - single frame
        Rectangle src = {
            0, 0,
            facingRight ? (float)BASE_W : -(float)BASE_W,
            (float)BASE_H
        };
        DrawTexturePro(texIdle, src, dest, {0, 0}, 0, WHITE);
    }
}

// --- damage overloads ---

void Player::TakeDamage(int amount) {
    if (isInvincible) return;   // no damage during grace period

    currentHearts -= amount;
    if (currentHearts < 0) currentHearts = 0;

    isInvincible = true;
    invincTimer  = INVINCIBLE_TIME;
    blinkTimer   = 0;
    blinkVisible = true;
}

void Player::TakeDamage(Obstacle& obs) {
    TakeDamage(obs.GetDamage());  // just forward to the int version
}

// --- getters ---

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

// slightly smaller than sprite for fair-feeling collisions
Rectangle Player::GetBounds() const {
    return Rectangle{
        position.x + 12,      // tighter sides
        position.y - 80,      // move UP (VERY IMPORTANT)
        36,
        60
    };
}

float Player::GetVelocityY() const { return velY; }

void Player::SetPositionY(float y)
{
    position.y = y;
    velY = 0;
}

void Player::SetPositionX(float x)
{
    position.x = x;
}

void Player::SetOnGround(bool val)
{
    isOnGround = val;
}