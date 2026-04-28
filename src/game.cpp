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

#include <vector>
#include <unordered_set>
#include <cstdlib>
#include <ctime>

void Game::Run() {
    InitWindow(1152, 528, "Kattar Karachi");
    SetTargetFPS(60);
    srand(time(NULL));

    Player player;
    player.Init({400, GROUND_Y});

    Map map;
    map.Init();

    TaskManager tm;
    tm.Init();

    ScoreManager sm;
    sm.StartTimer();

    UIManager ui;
    ui.Init();

    Shopper shopper;
    shopper.Init("assets/shopper/shopper.png");

    // SHOP ORDER (your required order)
    float shopX[3] = {1527, 225, 2640};

    // spawn first shopper
    shopper.Spawn(shopX[0], 351);


    Camera2D camera = {0};
    camera.target = player.GetPosition();
    camera.offset = { SCREEN_W / 2.0f, SCREEN_H / 2.0f };
    camera.zoom = 1.0f;

    std::vector<Obstacle*> obstacles;

    obstacles.push_back(new TrashPile({800, 417}));
    obstacles.push_back(new PushCart({1100, 391}));
    obstacles.push_back(new TrashPile({1350, 417}));
    obstacles.push_back(new Pothole({1650, 424}));
    obstacles.push_back(new PushCart({1900, 391}));
    obstacles.push_back(new TrashPile({2200, 417}));
    obstacles.push_back(new Pothole({2500, 424}));

    bool gameOver = false;
    std::unordered_set<Obstacle*> prevCollisions;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (!gameOver) {

            player.Update(dt);
            shopper.Update(dt);   // ⭐ IMPORTANT

            bool onPlatform = false;
            Rectangle pRect = player.GetBounds();
            std::unordered_set<Obstacle*> currentCollisions;

            // --- OBSTACLES ---
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
                            player.TakeDamage(*o);

                        float playerCenter = pRect.x + pRect.width / 2;
                        float obstacleCenter = oRect.x + oRect.width / 2;
                        float pushOffset = 8.0f;

                        if (playerCenter < obstacleCenter)
                            player.SetPositionX(oRect.x - pRect.width - pushOffset);
                        else
                            player.SetPositionX(oRect.x + oRect.width + pushOffset);

                        player.SetOnGround(true);
                    }
                }
                else
                {
                    if (o->IsLethal())
                    {
                        gameOver = true;
                        continue;
                    }

                    if (prevCollisions.count(o) == 0)
                        player.TakeDamage(*o);
                }
            }

            prevCollisions = currentCollisions;

            if (!onPlatform)
            {
                if (player.GetPosition().y < GROUND_Y)
                    player.SetOnGround(false);
            }

            if (!player.IsAlive())
                gameOver = true;

            // --- SHOPPER COLLISION (NEW SYSTEM) ---
            if (!tm.AllTasksDone() && shopper.IsActive()) {
                if (CheckCollisionRecs(pRect, shopper.GetBounds())) {

                    shopper.Deactivate();
                    tm.CompleteCurrentTask();

                    int next = tm.GetCompletedCount();

                    if (next < 3) {
                        shopper.Spawn(shopX[next], 351);
                    } else {
                        sm.StopTimer();
                    }
                }
            }

            // --- CAMERA ---
            float playerX = player.GetPosition().x;
            camera.target.x = Lerp(camera.target.x, playerX, 0.08f);

            float halfScreen = SCREEN_W / 2.0f;
            if (camera.target.x < halfScreen)
                camera.target.x = halfScreen;
            if (camera.target.x > WORLD_W - halfScreen)
                camera.target.x = WORLD_W - halfScreen;

            camera.target.y = SCREEN_H / 2.0f;
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        map.Draw();

        if (gameOver) {
            DrawText("GAME OVER", 450, 250, 40, RED);
        } else {

            for (auto o : obstacles)
                if (o->IsActive())
                    o->Draw();

            shopper.Draw();   // ⭐ NEW
            player.Draw();
        }

        EndMode2D();

        ui.DrawHUD(player, tm, sm, dt);

        EndDrawing();
    }

    for (auto o : obstacles) delete o;
    map.Unload();
    CloseWindow();
}