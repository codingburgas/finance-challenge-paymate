#include "budget.h"
#include "dashboard.h"
#include "statistics.h"
#include "profile.h"

void budget()
{
    const int screenWidth = 900;
    const int screenHeight = 1080;

    const Rectangle dashboardButton = { 50, 970, 140, 75 };
    const Rectangle budgetButton = { 280, 970, 140, 75 };
    const Rectangle statisticsButton = { 510, 970, 140, 75 };
    const Rectangle profileButton = { 740, 970, 140, 75 };

    float monthlyIncome = 41539.0f;
    float expenses = 20119.0f;
    float balance = 83542.97f;
    float growthRate = 9.5f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Marhaba, 6efa na Relefa", 50, 50, 20, DARKGRAY);
        DrawText("Good Afternoon", 50, 80, 18, GRAY);

        DrawText("Kesh Money", 50, 140, 18, BLUE);
        DrawText(TextFormat("$%.2f", balance), 50, 170, 40, BLACK);
        DrawText("Overview", 50, 220, 16, LIGHTGRAY);

        DrawText(TextFormat("+ %.1f%%", growthRate), 300, 170, 20, GREEN);

        Rectangle incomeBox = { 50, 300, 300, 100 };
        Rectangle expenseBox = { 380, 300, 300, 100 };

        DrawRectangleRounded(incomeBox, 0.2, 10, DARKBLUE);
        DrawText("Income", incomeBox.x + 20, incomeBox.y + 10, 20, RAYWHITE);
        DrawText(TextFormat("$%.2f", monthlyIncome), incomeBox.x + 20, incomeBox.y + 40, 25, RAYWHITE);

        DrawRectangleRounded(expenseBox, 0.2, 10, RED);
        DrawText("Expenses", expenseBox.x + 20, expenseBox.y + 10, 20, RAYWHITE);
        DrawText(TextFormat("$%.2f", expenses), expenseBox.x + 20, expenseBox.y + 40, 25, RAYWHITE);

        DrawText("Favorite People", 50, 450, 20, DARKGRAY);
        DrawCircle(100, 500, 20, LIGHTGRAY);
        DrawCircle(150, 500, 20, LIGHTGRAY);
        DrawCircle(200, 500, 20, LIGHTGRAY);
        DrawCircle(250, 500, 20, LIGHTGRAY);

        DrawText("Transaction", 50, 600, 20, DARKGRAY);

        int transactionY = 640;
        DrawText("$2,852", 50, transactionY, 18, BLACK);
        DrawText("Sent by Google", 200, transactionY, 18, GRAY);

        transactionY += 50;
        DrawText("$5,910", 50, transactionY, 18, BLACK);
        DrawText("Sent by Apple Pay", 200, transactionY, 18, GRAY);

        transactionY += 50;
        DrawText("$3,650", 50, transactionY, 18, BLACK);
        DrawText("Sent by PayPal", 200, transactionY, 18, GRAY);

        DrawRectangle(0, 930, 900, 200, BLACK);

        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, dashboardButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dashboard();
        }

        bool isMouseOverStatisticsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatisticsButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, statisticsButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            statistics();
        }

        bool isMouseOverProfileButton = CheckCollisionPointRec(mousePosition, profileButton);
        DrawRectangleRounded(profileButton, 10, int(2), (isMouseOverProfileButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, profileButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            profile();
        }

        EndDrawing();
    }
}
