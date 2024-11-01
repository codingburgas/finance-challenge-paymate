#include "budget.h"
#include "dashboard.h"
#include "statistics.h"
#include "profile.h"
#include "accessData.h"
#include "validation.h"

struct USER_DATA {
    float monthlyIncome;
    float savings;
    float balance;
    float growthRates;

};

extern USER_DATA loadUserData();
extern void updateBalance(float newBalance);

void budget() {
    const int screenWidth = 900;
    const int screenHeight = 1080;


    const Rectangle dashboardButton = { 150, 970, 140, 75 };
    const Rectangle budgetButton = { 380, 970, 140, 75 };
    const Rectangle statisticsButton = { 610, 970, 140, 75 };

    // Input box variables for four expenses
    char input1[25] = "\0";
    char input2[25] = "\0";
    char input3[25] = "\0";
    char input4[25] = "\0";

    int inputBox1LetterCount = 0;
    int inputBox2LetterCount = 0;
    int inputBox3LetterCount = 0;
    int inputBox4LetterCount = 0;

    bool mouseOnInputBox1 = false;
    bool mouseOnInputBox2 = false;
    bool mouseOnInputBox3 = false;
    bool mouseOnInputBox4 = false;

    Rectangle inputBox1 = { 200, 300, 550, 65 };
    Rectangle inputBox2 = { 200, 400, 550, 65 };
    Rectangle inputBox3 = { 200, 500, 550, 65 };
    Rectangle inputBox4 = { 200, 600, 550, 65 };
    Rectangle submitDataButton = { 500, 700, 140, 75 };

    USER_DATA userData = loadUserData();  // Load the user's data
    Texture2D manBigSize = LoadTexture("../images/m.png");
    Texture2D womanBigSize = LoadTexture("../images/w.png");

    int newWidth = manBigSize.width / 2 + 30;
    int newHeight = manBigSize.height / 2;
    Image manImage = LoadImage("../images/m.png");
    Image womanImage = LoadImage("../images/w.png");
    ImageResize(&manImage, newWidth - 15, newHeight);
    ImageResize(&womanImage, newWidth , newHeight);
    Texture2D man = LoadTextureFromImage(manImage);
    Texture2D woman = LoadTextureFromImage(womanImage);
    UnloadImage(manImage);
    UnloadImage(womanImage);

    Validate validator;
    const Rectangle picToProfile = { GetScreenWidth() / 2 + 250, GetScreenHeight() / 2 - 500, man.width, man.height };
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();

        // Check mouse position over input boxes
        mouseOnInputBox1 = CheckCollisionPointRec(mousePosition, inputBox1);
        mouseOnInputBox2 = CheckCollisionPointRec(mousePosition, inputBox2);
        mouseOnInputBox3 = CheckCollisionPointRec(mousePosition, inputBox3);
        mouseOnInputBox4 = CheckCollisionPointRec(mousePosition, inputBox4);

        // Set mouse cursor
        if (mouseOnInputBox1 || mouseOnInputBox2 || mouseOnInputBox3 || mouseOnInputBox4) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        }
        else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // Handle input for each box
        if (mouseOnInputBox1) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (inputBox1LetterCount < 24)) {
                    input1[inputBox1LetterCount++] = (char)key;
                    input1[inputBox1LetterCount] = '\0'; // Null-terminate the string
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputBox1LetterCount > 0) {
                input1[--inputBox1LetterCount] = '\0';
            }
        }

        if (mouseOnInputBox2) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (inputBox2LetterCount < 24)) {
                    input2[inputBox2LetterCount++] = (char)key;
                    input2[inputBox2LetterCount] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputBox2LetterCount > 0) {
                input2[--inputBox2LetterCount] = '\0';
            }
        }

        if (mouseOnInputBox3) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (inputBox3LetterCount < 24)) {
                    input3[inputBox3LetterCount++] = (char)key;
                    input3[inputBox3LetterCount] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputBox3LetterCount > 0) {
                input3[--inputBox3LetterCount] = '\0';
            }
        }

        if (mouseOnInputBox4) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (inputBox4LetterCount < 24)) {
                    input4[inputBox4LetterCount++] = (char)key;
                    input4[inputBox4LetterCount] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputBox4LetterCount > 0) {
                input4[--inputBox4LetterCount] = '\0';
            }
        }

        ClearBackground(RAYWHITE);
        DrawRectangle(0, 930, 900, 200, BLACK);
        if (validator.maleOrFemale(currentUser))
        {
            DrawTexture(man, GetScreenWidth() / 2 + 250, GetScreenHeight() / 2 - 500, RAYWHITE);
        }
        else
        {
            DrawTexture(woman, GetScreenWidth() / 2 + 250, GetScreenHeight() / 2 - 500, RAYWHITE);
        }
        bool isMouseOverProfilePic = CheckCollisionPointRec(mousePosition, picToProfile);
        if (isMouseOverProfilePic && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            profile();
        }
        DrawRectangleRec(inputBox1, LIGHTGRAY);
        DrawText("Expense 1:", inputBox1.x, inputBox1.y - 30, 20, DARKGRAY);
        DrawText(input1, inputBox1.x + 5, inputBox1.y + 8, 40, BLACK);

        DrawRectangleRec(inputBox2, LIGHTGRAY);
        DrawText("Expense 2:", inputBox2.x, inputBox2.y - 30, 20, DARKGRAY);
        DrawText(input2, inputBox2.x + 5, inputBox2.y + 8, 40, BLACK);

        DrawRectangleRec(inputBox3, LIGHTGRAY);
        DrawText("Expense 3:", inputBox3.x, inputBox3.y - 30, 20, DARKGRAY);
        DrawText(input3, inputBox3.x + 5, inputBox3.y + 8, 40, BLACK);

        DrawRectangleRec(inputBox4, LIGHTGRAY);
        DrawText("Expense 4:", inputBox4.x, inputBox4.y - 30, 20, DARKGRAY);
        DrawText(input4, inputBox4.x + 5, inputBox4.y + 8, 40, BLACK);

        // Handle submit button click
        DrawRectangleRounded(submitDataButton, 0.2, 10, DARKBLUE);
        DrawText("Submit", submitDataButton.x + 15, submitDataButton.y + 20, 25, RAYWHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, submitDataButton)) {
            try {
                // Calculate total expenses and update balance
                float totalExpense = stof(input1) + stof(input2) + stof(input3) + stof(input4);
                userData.balance -= totalExpense;  // Deduct expenses from balance
                updateBalance(userData.balance);   // Save updated balance to CSV
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid input for expenses\n";
            }
        }

        // Draw navigation buttons
        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        if (isMouseOverDashboardButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dashboard();
        }


        bool isMouseOverStatisticsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatisticsButton ? DARKGRAY : LIGHTGRAY));
        if (isMouseOverStatisticsButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            statistics();
        }

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        if (isMouseOverBudgetButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ;
        }

        EndDrawing();
    }
}
