#include "game.h"
#include "raylib.h"


void Game::Run() {


    // ---------------- INIT ----------------
    // setup window and FPS
    InitWindow(1152, 528, "Kattar Karachi");
    SetTargetFPS(60);


    // LoadAssets()
    // loads textures, sounds, fonts for game


    // InitObjects()
    // initialize player, map, obstacles, UI




    // ---------------- GAME LOOP ----------------
    while (!WindowShouldClose()) {


        // ---------------- UPDATE ----------------
        float dt = GetFrameTime();


        // Input()
        // reads keyboard input (left, right, jump)


        // PlayerMove(dt)
        // moves player left/right based on input


        // ApplyGravity(dt)
        // handles jumping and falling


        // CameraFollow(player.x)
        // moves camera with player (side scrolling logic)


        // CheckCollision(player, obstacles)
        // checks collision and reduces health


        // UpdateTasks(player.position)
        // completes task if player reaches location


        // UpdateScore()
        // updates score based on progress


        // UpdateUI()
        // updates hearts, task text, etc.




        // ---------------- DRAW ----------------
        BeginDrawing();


        // ClearScreen()
        ClearBackground(RAYWHITE);


        // DrawMap(camera)
        // draws background, buildings, road


        // DrawObstacles()
        // draws pothole, cart, trash, dog


        // DrawPlayer()
        // draws player sprite


        // DrawUI()
        // draws hearts, tasks, score


        // temporary text for testing
        DrawText("Kattar Karachi under construction !!!!", 0, 0, 50, BLACK);


        EndDrawing();




        // ---------------- GAME STATE ----------------
        // CheckGameOver()
        // if hearts == 0 → end game


        // CheckTaskComplete()
        // if all tasks done → win game
    }


    // ---------------- CLEANUP ----------------
    // free memory and close window
    CloseWindow();
}