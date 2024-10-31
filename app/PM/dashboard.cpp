#include "dashboard.h"
#include "budget.h"
#include "statistics.h"
#include "profile.h"
#include "accessData.h"
#include "validation.h"


struct USER_DATA {
    float monthlyIncome;
    float savings;
    float balance;
};

USER_DATA loadUserData() {
    USER_DATA userData;
    rapidcsv::Document accountData("../data/accountData.csv");

    try {
        vector<string> usernames = accountData.GetColumn<string>("username");
        int userRow = -1;
        for (int i = 0; i < usernames.size(); i++) {
            if (usernames[i] == currentUser) {
                userRow = i;
                break;
            }
        }
        if (userRow != -1) {
            userData.monthlyIncome = stof(accountData.GetCell<string>("monthlyIncome", userRow));
            userData.savings = stof(accountData.GetCell<string>("savings", userRow));
            userData.balance = stof(accountData.GetCell<string>("balance", userRow));
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading user data: " << e.what() << '\n';
    }

    return userData;
}

void updateBalance(float newBalance) {
    rapidcsv::Document accountData("../data/accountData.csv");

    vector<string> usernames = accountData.GetColumn<string>("username");
    int userRow = -1;
    for (int i = 0; i < usernames.size(); i++) {
        if (usernames[i] == currentUser) {
            userRow = i;
            break;
        }
    }

    if (userRow != -1) {
        accountData.SetCell("balance", userRow, std::to_string(newBalance));
        accountData.Save("../data/accountData.csv");
    }
}

// Variables for wage input box
char wageInput[25] = "\0";
Rectangle wageInputBox = { 50, 330, 300, 50 };
bool mouseOnWageInputBox = false;
int wageInputLetterCount = 0;

void dashboard()
{
    const int screenWidth = 900;
    const int screenHeight = 1080;

    Rectangle balanceBox = { 50, 200, 300, 100 };
    Rectangle savingsBox = { 380, 200, 300, 100 };

    const Rectangle dashboardButton = { 50, 970, 140, 75 };
    const Rectangle budgetButton = { 280, 970, 140, 75 };
    const Rectangle statisticsButton = { 510, 970, 140, 75 };
    const Rectangle profileButton = { 740, 970, 140, 75 };

    USER_DATA userData = loadUserData();
    Texture2D manBigSize = LoadTexture("../images/m.png");
    Texture2D womanBigSize = LoadTexture("../images/w.png");

    int newWidth = manBigSize.width / 2+30;
    int newHeight = manBigSize.height / 2;
    Image manImage = LoadImage("../images/m.png");
    Image womanImage = LoadImage("../images/w.png");
    ImageResize(&manImage, newWidth, newHeight);
    ImageResize(&womanImage, newWidth, newHeight);
    Texture2D man = LoadTextureFromImage(manImage);
    Texture2D woman = LoadTextureFromImage(womanImage);
    UnloadImage(manImage);
    UnloadImage(womanImage);

    Validate validator;
    const Rectangle picToProfile = { GetScreenWidth() / 2 + 250, GetScreenHeight() / 2 - 500, man.width, man.height };
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();

        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (CheckCollisionPointRec(mousePosition, wageInputBox)) mouseOnWageInputBox = true;
        else mouseOnWageInputBox = false;

        if (mouseOnWageInputBox) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (wageInputLetterCount < 25)) {
                    wageInput[wageInputLetterCount] = (char)key;
                    wageInput[wageInputLetterCount + 1] = '\0';
                    wageInputLetterCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                wageInputLetterCount--;
                if (wageInputLetterCount < 0) wageInputLetterCount = 0;
                wageInput[wageInputLetterCount] = '\0';
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
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
        DrawText("Hello, ", 50, 80, 80, DARKGRAY);
        DrawText(currentUser, 300, 80, 80, DARKGRAY);

        // Display balance
        DrawRectangleRounded(balanceBox, 0.2, 10, DARKBLUE);
        DrawText("Balance", balanceBox.x + 20, balanceBox.y + 10, 20, RAYWHITE);
        DrawText(TextFormat("$%.2f", userData.balance), balanceBox.x + 20, balanceBox.y + 40, 25, RAYWHITE);

        DrawRectangleRounded(savingsBox, 0.2, 10, RED);
        DrawText("Savings", savingsBox.x + 20, savingsBox.y + 10, 20, RAYWHITE);
        DrawText(TextFormat("$%.2f", userData.savings), savingsBox.x + 20, savingsBox.y + 40, 25, RAYWHITE);

        // Wage input box
        DrawRectangleRec(wageInputBox, LIGHTGRAY);
        DrawText("Enter Wage:", wageInputBox.x, wageInputBox.y - 30, 20, DARKGRAY);
        DrawText(wageInput, wageInputBox.x + 5, wageInputBox.y + 8, 40, BLACK);

        if (mouseOnWageInputBox && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            try {
                userData.balance = stof(wageInput);  // Update balance with wage input
                updateBalance(userData.balance);  // Save new balance to CSV
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid input for wage\n";
            }
        }

        // Navigation buttons
        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        if (isMouseOverBudgetButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            budget();
        }

        bool isMouseOverStatisticsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatisticsButton ? DARKGRAY : LIGHTGRAY));
        if (isMouseOverStatisticsButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            statistics();
        }

        bool isMouseOverProfileButton = CheckCollisionPointRec(mousePosition, profileButton);
        DrawRectangleRounded(profileButton, 10, int(2), (isMouseOverProfileButton ? DARKGRAY : LIGHTGRAY));
        if (isMouseOverProfileButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            profile();
        }

        EndDrawing();
    }
}