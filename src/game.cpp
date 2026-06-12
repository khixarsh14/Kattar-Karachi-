#include "game.h"
#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "map.h"
#include "TaskManager.h"
#include "scoremanager.h"
#include "UImanager.h"
#include "Pothole.h"
#include "PushCart.h"
#include "TrashPile.h"
#include "shopper.h"
#include "audiomanager.h"

#include <vector>
#include <unordered_set>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

int LoadHighScore()
{
    std::ifstream file("saves/highscores.txt");
    int score = 0;
    if (file >> score) return score;
    return 0;
}

void SaveHighScore(int score)
{
    std::ofstream file("saves/highscores.txt");
    file << score;
}

void Game::Run()
{
// Returns the scale rect for drawing renderTarget centered in the window
static void DrawScaled(RenderTexture2D& rt) {
    int winW = GetScreenWidth();
    int winH = GetScreenHeight();
    float scaleX = (float)winW / 1152;
    float scaleY = (float)winH / 528;
    float scale  = (scaleX < scaleY) ? scaleX : scaleY;
    float drawW  = 1152 * scale;
    float drawH  = 528  * scale;
    float offX   = (winW - drawW) / 2.0f;
    float offY   = (winH - drawH) / 2.0f;

    // NOTE: negative height in src flips the texture (Raylib stores it upside-down)
    Rectangle src  = { 0, 0, 1152, -528 };
    Rectangle dest = { offX, offY, drawW, drawH };
    DrawTexturePro(rt.texture, src, dest, {0,0}, 0.0f, WHITE);
}

// Converts real mouse position → game-space position (accounts for scale + letterbox)
static Vector2 GetScaledMouse(RenderTexture2D& rt) {
    int winW = GetScreenWidth();
    int winH = GetScreenHeight();
    float scaleX = (float)winW / 1152;
    float scaleY = (float)winH / 528;
    float scale  = (scaleX < scaleY) ? scaleX : scaleY;
    float offX   = (winW - 1152 * scale) / 2.0f;
    float offY   = (winH - 528  * scale) / 2.0f;
    Vector2 m    = GetMousePosition();
    return { (m.x - offX) / scale, (m.y - offY) / scale };
}

void Game::Run() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1152, 528, "Kattar Karachi");
    SetWindowMinSize(576, 264);   // minimum = half game size
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    srand(time(NULL));
    // Fixed-resolution render target — game always draws at 1152×528
    RenderTexture2D renderTarget = LoadRenderTexture(1152, 528);
    SetTextureFilter(renderTarget.texture, TEXTURE_FILTER_POINT); // keeps pixels sharp
    //audios
    InitAudioDevice();

    Sound sndButton = LoadSound("assets/sounds/button.mp3");

    Texture2D texStart = LoadTexture("assets/backgrounds/start.png");
    Texture2D texWon   = LoadTexture("assets/backgrounds/won.png");
    Texture2D texOver  = LoadTexture("assets/backgrounds/over.png");
    Texture2D startClouds = LoadTexture("assets/backgrounds/clouds.png");

    float cloudOffset = 0.0f;
    const float cloudSpeed = 20.0f;
    const float cloudScale = 1.5f;  

    const int BTN_W = 160;
    const int BTN_H = 50;

    Rectangle playBtn = {
        (1152 - BTN_W) / 2.0f,
        (528 - BTN_H) / 2.0f - 16,
        (float)BTN_W,
        (float)BTN_H
    };

    Rectangle restartBtn = {
        1152 / 2.0f - 230.0f,
        300.0f,
        184.0f,
        55.0f
    };

    Rectangle leaveBtn = {
        1152 / 2.0f + 48.0f,
        300.0f,
        184.0f,
        55.0f
    };

    GameState state = STATE_START;

    Player player;
    Map map;
    TaskManager tm;
    ScoreManager sm;
    UIManager ui;
    AudioManager audio;
    Shopper shopper;

    float shopX[3] = {1527, 225, 2640};

    Camera2D camera = {0};
    camera.offset = {SCREEN_W / 2.0f, SCREEN_H / 2.0f};
    camera.zoom = 1.0f;
    camera.target = {400, SCREEN_H / 2.0f};

    std::vector<Obstacle*> obstacles;
    std::unordered_set<Obstacle*> prevCollisions;

    int finalScore = 0;
    int highScore = LoadHighScore();
    bool audioReady = false;
    bool mapReady = false;

    auto ClearObstacles = [&]() {
        for (auto o : obstacles) delete o;
        obstacles.clear();
        prevCollisions.clear();
    };

    auto StartGame = [&]() {
        ClearObstacles();

        if (!mapReady) {
            map.Init();
            mapReady = true;
        }

        if (!audioReady) {
            audio.Init();
            audioReady = true;
        }

        audio.RestartMusic();

        player = Player();
        player.Init({400, GROUND_Y});

        tm = TaskManager();
        tm.Init();

        sm = ScoreManager();
        sm.StartTimer();

        ui = UIManager();
        ui.Init();

        shopper = Shopper();
        shopper.Init("assets/shopper/shopper.png");
        shopper.Spawn(shopX[0], 351);

        camera.target = {400, SCREEN_H / 2.0f};

        obstacles.push_back(new TrashPile({800, 417}));
        obstacles.push_back(new PushCart({1100, 391}));
        obstacles.push_back(new TrashPile({1350, 417}));
        obstacles.push_back(new Pothole({1650, 424}));
        obstacles.push_back(new PushCart({1900, 391}));
        obstacles.push_back(new TrashPile({2200, 417}));
        obstacles.push_back(new Pothole({2500, 424}));

        finalScore = 0;
        state = STATE_PLAYING;
    };

    auto FinishRun = [&](GameState endState) {
        sm.StopTimer();
        sm.CalculateScore(tm);
        finalScore = sm.GetScore();

        if (finalScore > highScore) {
            highScore = finalScore;
            SaveHighScore(highScore);
        }

        if (endState == STATE_GAMEOVER) {
            audio.PlayGameOver();
        }

        state = endState;
    };

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        float cloudTileW = startClouds.width * cloudScale;

        cloudOffset += cloudSpeed * dt;

        while (cloudOffset >= cloudTileW)
            cloudOffset -= cloudTileW;
        //  START SCREEN
        if (state == STATE_START) {
            // Draw to render texture
            BeginTextureMode(renderTarget);
            ClearBackground(BLACK);
            DrawTexture(texStart, 0, 0, WHITE);
            if (CheckCollisionPointRec(GetScaledMouse(renderTarget), playBtn))
                DrawRectangleRec(playBtn, {255, 255, 255, 40});
            EndTextureMode();

            // Scale to window
            BeginDrawing();
            ClearBackground(BLACK);
            DrawScaled(renderTarget);
            EndDrawing();

        if (state == STATE_START)
        {
            if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), playBtn)) ||
                IsKeyPressed(KEY_SPACE))
            {
                PlaySound(sndButton);
                StartGame() ;
            }
        }
        else if (state == STATE_PLAYING)
        {
            audio.Update();

        // F11 = toggle fullscreen
        if (IsKeyPressed(KEY_F11)) {
            if (IsWindowFullscreen()) {
                ToggleFullscreen();
                SetWindowSize(1152, 528);
            } else {
                int mon = GetCurrentMonitor();
                SetWindowSize(GetMonitorWidth(mon), GetMonitorHeight(mon));
                ToggleFullscreen();
            }
        }
        //  GAME OVER SCREEN
        if (state == STATE_GAMEOVER) {
            BeginTextureMode(renderTarget);
            ClearBackground(BLACK);
            DrawTexture(texEnd, 0, 0, WHITE);

            // Centred score & time
            const char* scoreText = TextFormat("Score: %d", finalScore);
            const char* timeText  = TextFormat("Time:  %.1f s", sm.GetElapsedTime());

            int sw = MeasureText(scoreText, 36);
            int tw = MeasureText(timeText,  28);

            int cx = 1152 / 2;
            int cy = 528  / 2;

            // shadow + text for score
            DrawText(scoreText, cx - sw/2 + 2, cy - 30 + 2, 36, BLACK);
            DrawText(scoreText, cx - sw/2,     cy - 30,     36, YELLOW);

            // shadow + text for time
            DrawText(timeText,  cx - tw/2 + 2, cy + 20 + 2, 28, BLACK);
            DrawText(timeText,  cx - tw/2,     cy + 20,     28, WHITE);
            EndTextureMode();

            // Scale to window
            BeginDrawing();     
            ClearBackground(BLACK);
            DrawScaled(renderTarget);
            EndDrawing();

            //close window
            if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                audio.Unload();
                map.Unload();
                state = STATE_START;
            }
            continue;
        }

        //  PLAYING
        audio.Update();

        if (!gameOver) {
            player.Update(dt);
            shopper.Update(dt);

            bool onPlatform = false;
            Rectangle pRect = player.GetBounds();
            std::unordered_set<Obstacle*> currentCollisions;

            for (auto o : obstacles)
            {
                if (!o->IsActive()) continue;

                Rectangle oRect = o->GetBounds();

                if (!CheckCollisionRecs(pRect, oRect))
                    continue;

                currentCollisions.insert(o);

                if (PushCart* cart = dynamic_cast<PushCart*>(o))
                {
                    if (IsLandingOnTop(pRect, oRect, player.GetVelocityY()))
                    {
                        player.SetPositionY(oRect.y + 5);
                        player.SetOnGround(true);
                        onPlatform = true;
                    }
                    else
                    {
                        if (prevCollisions.count(o) == 0)
                        {
                            player.TakeDamage(*o);
                            audio.PlayObstacleSound(1);
                        }

                        float playerCenter = pRect.x + pRect.width / 2;
                        float obstacleCenter = oRect.x + oRect.width / 2;
                        float pushOffset = 8.0f;

                        if (playerCenter < obstacleCenter)
                            player.SetPositionX(oRect.x - pRect.width - pushOffset);
                        else
                            player.SetPositionX(oRect.x + oRect.width + pushOffset);
                    }
                }
                else
                {
                    if (prevCollisions.count(o) == 0)
                    {
                        player.TakeDamage(*o);
                        audio.PlayObstacleSound(o->GetObstacleType());
                    }

                    if (o->IsLethal())
                    {
                        FinishRun(STATE_GAMEOVER);
                        break;
                    }
                }
            }

            if (state == STATE_PLAYING)
            {
                prevCollisions = currentCollisions;

                if (!onPlatform && player.GetPosition().y < GROUND_Y)
                    player.SetOnGround(false);

                if (!player.IsAlive())
                {
                    FinishRun(STATE_GAMEOVER);
                }

                if (state == STATE_PLAYING && !tm.AllTasksDone() && shopper.IsActive())
                {
                    if (CheckCollisionRecs(pRect, shopper.GetBounds()))
                    {
                        shopper.Deactivate();
                        tm.CompleteCurrentTask();
                        audio.PlayTaskComplete();

                        int next = tm.GetCompletedCount();
                        if (next < 3)
                            shopper.Spawn(shopX[next], 351);
                        else
                            FinishRun(STATE_WON);
                    }
                }

                if (state == STATE_PLAYING)
                {
                    float playerX = player.GetPosition().x;
                    Vector2 targetCam = {playerX, SCREEN_H / 2.0f};

                    float halfScreen = SCREEN_W / 2.0f;
                    targetCam.x = Clamp(targetCam.x, halfScreen, WORLD_W - halfScreen);

                    camera.target.x = Lerp(camera.target.x, targetCam.x, 0.15f);
                    camera.target.y = Lerp(camera.target.y, targetCam.y, 0.15f);

                    map.Update(camera.target.x);
                }
            }
        }
        else if (state == STATE_WON || state == STATE_GAMEOVER)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mouse = GetMousePosition();

                if (CheckCollisionPointRec(mouse, restartBtn))
                {
                    StartGame();
                }
                else if (CheckCollisionPointRec(mouse, leaveBtn))
                {
                    break;
                }
            }
            
            if (IsKeyPressed(KEY_SPACE))
            {
                StartGame();
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                break;
            }
        }

        BeginDrawing();
        // Draw — game renders to fixed 1152×528 texture
        BeginTextureMode(renderTarget);
        ClearBackground(RAYWHITE);

        if (state == STATE_START)
        {
            DrawTexture(texStart, 0, 0, WHITE);

            float cloudTileW = startClouds.width * cloudScale;
            float startX =- cloudOffset;
            for (int i = -1; i <= 2; i++)
            {
                DrawTextureEx(
                    startClouds,
                    { startX + i * cloudTileW, 0 },
                    0,
                    cloudScale,
                    WHITE
                );
            }
        }
        else if (state == STATE_PLAYING)
        {
            BeginMode2D(camera);

            map.Draw(camera);

            for (auto o : obstacles)
                if (o->IsActive())
                    o->Draw();

            shopper.Draw();
            player.Draw();

            EndMode2D();

            ui.DrawHUD(player, tm, sm, dt);
        }
        else if (state == STATE_WON || state == STATE_GAMEOVER)
        {
            Texture2D bg = (state == STATE_WON) ? texWon : texOver;
            DrawTexture(bg, 0, 0, WHITE);

            char scoreText[64];
            char highText[64];
            snprintf(scoreText, sizeof(scoreText), "Your Score = %d", finalScore);
            snprintf(highText, sizeof(highText), "High Score = %d", highScore);

            int scoreW = MeasureText(scoreText, 32);
            int highW = MeasureText(highText, 28);

            int cx = 1152 / 2;
            int scoreY = 207;
            int highY = 242;

            DrawText(scoreText, cx - scoreW / 2 + 2, scoreY + 2, 32, BLACK);
            DrawText(scoreText, cx - scoreW / 2, scoreY, 32, YELLOW);

            DrawText(highText, cx - highW / 2 + 2, highY + 2, 28, BLACK);
            DrawText(highText, cx - highW / 2, highY, 28, WHITE);

            //DrawRectangleRec(restartBtn, Fade(BLACK, 0.35f));
            //DrawRectangleRec(leaveBtn, Fade(BLACK, 0.35f));

        }
        ui.DrawHUD(player, tm, sm, dt);
        EndTextureMode();

        // Scale texture to actual window size
        BeginDrawing();
        ClearBackground(BLACK);
        DrawScaled(renderTarget);
        EndDrawing();
    }

    ClearObstacles();

    if (audioReady)
    {
        audio.Unload();
    }

    if (mapReady)
    {
        map.Unload();
    }

    UnloadRenderTexture(renderTarget);
    UnloadSound(sndButton);
    UnloadTexture(texStart);
    UnloadTexture(texWon);
    UnloadTexture(texOver);
    UnloadTexture(startClouds);

    CloseAudioDevice();
    CloseWindow();
}