#include "pch.h"


int main()
{
   
    const int screenWidth = 900;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "PM");

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("PayMate", 190, 200, 20, LIGHTGRAY);
        EndDrawing();

    }
    CloseWindow();
}

    