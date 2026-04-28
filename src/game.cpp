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

void Game::Run() {
    InitWindow(1152, 528, "Kattar Karachi");
    SetTargetFPS(60);
    srand(time(NULL));

    //audios
    InitAudioDevice();
    Sound sndButton = LoadSound("assets/sounds/button.mp3");

    //Screen Texture
    Texture2D texStart = LoadTexture("assets/backgrounds/start.png");
    Texture2D texEnd   = LoadTexture("assets/backgrounds/end.png");

    // PLAY button
    const int   BTN_W = 205;
    const int   BTN_H = 75;
    Rectangle   playBtn = {
        (1152 - BTN_W) / 2.0f,
        (528  - BTN_H) / 2.0f,
        (float)BTN_W,
        (float)BTN_H
    };

    GameState state = STATE_START;

    //Objects
    Player player;
    Map    map;
    TaskManager tm;
    ScoreManager sm;
    UIManager ui;
    AudioManager audio;
    Shopper shopper;

    float shopX[3] = {1527, 225, 2640};

    Camera2D camera = {0};
    camera.offset   = { SCREEN_W / 2.0f, SCREEN_H / 2.0f };
    camera.zoom     = 1.0f;

    std::vector<Obstacle*> obstacles;
    std::unordered_set<Obstacle*> prevCollisions;

    bool gameOver = false;
    int  finalScore = 0;

    // Lambda to (re)init everything when PLAY is pressed
    auto StartGame = [&]() {
        // clear previous run
        for (auto o : obstacles) delete o;
        obstacles.clear();
        prevCollisions.clear();
        gameOver = false;

        player.Init({400, GROUND_Y});
        map.Init();
        tm.Init();
        sm.StartTimer();
        ui.Init();
        audio.Init();
        shopper.Init("assets/shopper/shopper.png");
        shopper.Spawn(shopX[0], 351);

        camera.target = {400, SCREEN_H / 2.0f};

        obstacles.push_back(new TrashPile({800,  417}));
        obstacles.push_back(new PushCart ({1100, 391}));
        obstacles.push_back(new TrashPile({1350, 417}));
        obstacles.push_back(new Pothole  ({1650, 424}));
        obstacles.push_back(new PushCart ({1900, 391}));
        obstacles.push_back(new TrashPile({2200, 417}));
        obstacles.push_back(new Pothole  ({2500, 424}));

        state = STATE_PLAYING;
    };





    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        //  START SCREEN
        if (state == STATE_START) {
            BeginDrawing();
            DrawTexture(texStart, 0, 0, WHITE);

            // Optional subtle hover tint so the player knows the button is clickable
            if (CheckCollisionPointRec(GetMousePosition(), playBtn))
                DrawRectangleRec(playBtn, {255, 255, 255, 40});

            EndDrawing();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                CheckCollisionPointRec(GetMousePosition(), playBtn))
            {
                PlaySound(sndButton);
                StartGame();
            }
            continue;
        }

        //  GAME OVER SCREEN
        if (state == STATE_GAMEOVER) {
            BeginDrawing();
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

            for (auto o : obstacles) {
                if (!o->IsActive()) continue;
                Rectangle oRect = o->GetBounds();
                if (!CheckCollisionRecs(pRect, oRect)) continue;

                currentCollisions.insert(o);

                if (PushCart* cart = dynamic_cast<PushCart*>(o)) {
                    if (IsLandingOnTop(pRect, oRect, player.GetVelocityY())) {
                        player.SetPositionY(oRect.y + 5);
                        player.SetOnGround(true);
                        onPlatform = true;
                    } else {
                        if (prevCollisions.count(o) == 0) {
                            player.TakeDamage(*o);
                            audio.PlayObstacleSound(1);
                        }
                        float playerCenter   = pRect.x + pRect.width  / 2;
                        float obstacleCenter = oRect.x + oRect.width   / 2;
                        float pushOffset = 8.0f;
                        if (playerCenter < obstacleCenter)
                            player.SetPositionX(oRect.x - pRect.width - pushOffset);
                        else
                            player.SetPositionX(oRect.x + oRect.width + pushOffset);
                    }
                } else {
                    if (prevCollisions.count(o) == 0) {
                        player.TakeDamage(*o);
                        audio.PlayObstacleSound(o->GetObstacleType());
                    }
                    if (o->IsLethal()) { gameOver = true; continue; }
                }
            }

            prevCollisions = currentCollisions;

            if (!onPlatform && player.GetPosition().y < GROUND_Y)
                player.SetOnGround(false);

            if (!player.IsAlive()) gameOver = true;

            // Shopper collision
            if (!tm.AllTasksDone() && shopper.IsActive()) {
                if (CheckCollisionRecs(pRect, shopper.GetBounds())) {
                    shopper.Deactivate();
                    tm.CompleteCurrentTask();
                    audio.PlayTaskComplete();

                    int next = tm.GetCompletedCount();
                    if (next < 3)
                        shopper.Spawn(shopX[next], 351);
                    else
                        sm.StopTimer();
                }
            }

            // Camera
            float playerX = player.GetPosition().x;
            Vector2 targetCam = {playerX, SCREEN_H / 2.0f};
            float halfScreen  = SCREEN_W / 2.0f;
            targetCam.x = Clamp(targetCam.x, halfScreen, WORLD_W - halfScreen);
            camera.target.x = Lerp(camera.target.x, targetCam.x, 0.15f);
            camera.target.y = Lerp(camera.target.y, targetCam.y, 0.15f);

            map.Update(camera.target.x);

            // Transition to end screen instantly
            if (gameOver) {
                sm.StopTimer();
                sm.CalculateScore(tm);
                finalScore = sm.GetScore();
                audio.PlayGameOver();
                map.Unload();
                state = STATE_GAMEOVER;
                gameOver = false;
                continue;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);
        map.Draw(camera);

        for (auto o : obstacles)
            if (o->IsActive()) o->Draw();
        shopper.Draw();
        player.Draw();

        EndMode2D();

        ui.DrawHUD(player, tm, sm, dt);

        EndDrawing();
    }

    // Cleanup
    for (auto o : obstacles) delete o;
    if (state == STATE_PLAYING) {
        audio.Unload();
        map.Unload();
    }

    UnloadSound(sndButton);
    CloseAudioDevice();
    UnloadTexture(texStart);
    UnloadTexture(texEnd);
    CloseWindow();
}