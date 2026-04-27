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

    // 🔥 NEW: track collisions
    std::unordered_set<Obstacle*> prevCollisions;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if (!gameOver) {

            player.Update(dt);

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

                // -------- PUSH CART --------
                if (PushCart* cart = dynamic_cast<PushCart*>(o))
                {
                    if (IsLandingOnTop(pRect, oRect, player.GetVelocityY()))
                    {
                        // ✅ stable landing (no flicker)
                        player.SetPositionY(oRect.y + 5);
                        player.SetOnGround(true);
                        onPlatform = true;
                    }
                    else
                    {
                        // ✅ damage only once per contact
                        if (prevCollisions.count(o) == 0)
                            player.TakeDamage(*o);

                        // side push
                        float playerCenter = pRect.x + pRect.width / 2;
                        float obstacleCenter = oRect.x + oRect.width / 2;

                        float pushOffset = 8.0f;  // extra spacing to prevent sticking

                        if (playerCenter < obstacleCenter)
                        {
                            player.SetPositionX(oRect.x - pRect.width - pushOffset);
                        }
                        else
                        {
                            player.SetPositionX(oRect.x + oRect.width + pushOffset);
                        }

                        // 🔥 CRITICAL: stop horizontal movement
                        // prevents re-collision every frame
                        player.SetOnGround(true);
                    }
                }
                // -------- OTHER OBSTACLES --------
                else
                {
                    if (o->IsLethal())
                    {
                        gameOver = true;
                        continue;
                    }

                    // ✅ damage once per contact
                    if (prevCollisions.count(o) == 0)
                        player.TakeDamage(*o);
                }
            }

            // update collision memory
            prevCollisions = currentCollisions;

            if (!onPlatform)
            {
                if (player.GetPosition().y < GROUND_Y)
                    player.SetOnGround(false);
            }

            if (!player.IsAlive())
                gameOver = true;

            // camera
            float targetX = player.GetPosition().x;
            camera.target.x = Lerp(camera.target.x, targetX, 0.08f);

            float halfScreen = SCREEN_W / 2.0f;

            if (camera.target.x < halfScreen)
                camera.target.x = halfScreen;
            if (camera.target.x > WORLD_W - halfScreen)
                camera.target.x = WORLD_W - halfScreen;

            camera.target.y = SCREEN_H / 2.0f;

            // tasks
            float px = player.GetPosition().x;
            int target = tm.GetActiveShopID();

            if (target == 0 && px >= 75 && px <= 195) tm.CompleteCurrentTask();
            else if (target == 1 && px >= 846 && px <= 966) tm.CompleteCurrentTask();
            else if (target == 2 && px >= 521 && px <= 641) tm.CompleteCurrentTask();

            if (tm.AllTasksDone())
                sm.StopTimer();
        }

        // --- DRAW ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        if (gameOver) {
            map.Draw();
            DrawText("GAME OVER", 450, 250, 40, RED);
        } else {
            map.Draw();

            for (auto o : obstacles)
                if (o->IsActive())
                    o->Draw();

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