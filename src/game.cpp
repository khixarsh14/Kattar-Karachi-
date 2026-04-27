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
    camera.offset = { 1152 / 2.0f, 528 / 2.0f };
    camera.zoom = 1.0f;

    std::vector<Obstacle*> obstacles;

    // --- SECTION 1 (easy start) ---
    obstacles.push_back(new TrashPile({800, 417}));

    // --- SECTION 2 ---
    obstacles.push_back(new PushCart({1100, 393}));
    obstacles.push_back(new TrashPile({1350, 417}));

    // --- SECTION 3 ---
    obstacles.push_back(new Pothole({1650, 429}));
    obstacles.push_back(new PushCart({1900, 393}));

    // --- SECTION 4 ---
    obstacles.push_back(new TrashPile({2200, 417}));
    obstacles.push_back(new Pothole({2500, 429}));

    bool gameOver = false;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // -------- UPDATE --------
        player.Update(dt);

        // -------- COLLISIONS --------
        for (auto o : obstacles)
        {
            if (!o->IsActive()) continue;

            Rectangle p = player.GetBounds();
            Rectangle ob = o->GetBounds();

            if (!CheckCollisionRecs(p, ob)) continue;

            // ===== PUSHCART SPECIAL LOGIC =====
            if (dynamic_cast<PushCart*>(o))
            {
                // ✅ LAND ON TOP
                if (IsLandingOnTop(p, ob, player.GetVelocityY()))
                {
                    player.SetPositionY(ob.y);   // stand on top
                    player.SetOnGround(true);
                }
                else
                {
                    // ✅ SIDE HIT → DAMAGE + BLOCK
                    player.TakeDamage(*o);

                    if (p.x < ob.x)
                        player.SetPositionX(ob.x - p.width); // left block
                    else
                        player.SetPositionX(ob.x + ob.width); // right block
                }
            }
            else
            {
                // ===== NORMAL OBSTACLES =====
                if (o->IsLethal())
                {
                    gameOver = true;
                }
                else
                {
                    player.TakeDamage(*o);
                }
            }
        }

        if (!player.IsAlive()) {
            gameOver = true;
        }

        // -------- CAMERA (SMOOTH + CLAMPED) --------
        float targetX = player.GetPosition().x;

        // smooth follow (lerp)
        camera.target.x = Lerp(camera.target.x, targetX, 0.08f);

        // clamp inside map (map = 3072 width)
        float halfScreen = 1152 / 2.0f;

        if (camera.target.x < halfScreen)
            camera.target.x = halfScreen;

        if (camera.target.x > 3072 - halfScreen)
            camera.target.x = 3072 - halfScreen;

        camera.target.y = 528 / 2.0f;

        // -------- TASK SYSTEM --------
        float px = player.GetPosition().x;
        int target = tm.GetActiveShopID();

        if (target == 0 && px >= 521) tm.CompleteCurrentTask();
        else if (target == 1 && px >= 846) tm.CompleteCurrentTask();
        else if (target == 2 && px >= 1475) tm.CompleteCurrentTask();

        if (tm.AllTasksDone()) {
            sm.StopTimer();
        }

        // -------- DRAW --------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        if (gameOver) {
            DrawText("GAME OVER", 450, 250, 40, RED);
            EndDrawing();
            continue;
        }

        map.Draw();
        player.Draw();

        // draw obstacles
        for (auto o : obstacles) {
            if (o->IsActive())
                o->Draw();
        }

        EndMode2D();

        ui.DrawHUD(player, tm, sm, dt);

        EndDrawing();
    }

    for (auto o : obstacles) delete o;
    map.Unload();
    CloseWindow();
}