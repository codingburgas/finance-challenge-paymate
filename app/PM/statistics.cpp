#include "statistics.h"
#include "dashboard.h"
#include "budget.h"
#include "profile.h"

void statistics()
{
    const int screenWidth = 900;
    const int screenHeight = 1080;

    const Rectangle dashboardButton = { 50, 970, 140, 75 };
    const Rectangle budgetButton = { 280, 970, 140, 75 };
    const Rectangle statisticsButton = { 510, 970, 140, 75 };
    const Rectangle profileButton = { 740, 970, 140, 75 };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle(0, 930, 900, 200, BLACK);

        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, dashboardButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dashboard();
        }

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, budgetButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            budget();
        }

        bool isMouseOverProfileButton = CheckCollisionPointRec(mousePosition, profileButton);
        DrawRectangleRounded(profileButton, 10, int(2), (isMouseOverProfileButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, profileButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            profile();
        }

        EndDrawing();

    }
}