#include "game.h"
#include "raylib.h"

void Game::Run() {

    InitWindow(1152, 528, "Kattar Karachi");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Kattar Karachi under construction", 0, 0, 50, BLACK);

        EndDrawing();
    }

    CloseWindow();
}