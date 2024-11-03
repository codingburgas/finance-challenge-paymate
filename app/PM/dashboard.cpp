#include "dashboard.h"
#include "budget.h"
#include "statistics.h"
#include "profile.h"
#include "accessData.h"
#include "validation.h"

struct USER_DATA
{
    float monthlyIncome;
    float savings;
    float balance;
};

USER_DATA loadUserData()
{
    USER_DATA userData;
    rapidcsv::Document accountData("../data/accountData.csv");

    try
    {
        vector<string> usernames = accountData.GetColumn<string>("username");
        int userRow = -1;
        for (int i = 0; i < usernames.size(); i++)
        {
            if (usernames[i] == currentUser)
            {
                userRow = i;
                break;
            }
        }

        if (userRow != -1)
        {
            userData.monthlyIncome = stof(accountData.GetCell<string>("monthlyIncome", userRow));
            userData.savings = stof(accountData.GetCell<string>("savings", userRow));
            userData.balance = stof(accountData.GetCell<string>("balance", userRow));
        }
    }
    catch (const exception& e)
    {
        cerr << "Error loading user data: " << e.what() << '\n';
    }
    return userData;
}

void updateBalance(float newBalance)
{
    rapidcsv::Document accountData("../data/accountData.csv");

    vector<string> usernames = accountData.GetColumn<string>("username");
    int userRow = -1;
    for (int i = 0; i < usernames.size(); i++)
    {
        if (usernames[i] == currentUser)
        {
            userRow = i;
            break;
        }
    }

    if (userRow != -1)
    {
        accountData.SetCell("balance", userRow, to_string(newBalance));
        accountData.Save("../data/accountData.csv");
    }
}

struct Expense {
    float amount;
    float percentage;
};

vector<Expense> loadLastSpendings() {
    vector<Expense> spendings;
    ifstream file("../data/lastSpendings.csv");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string amountStr, percentageStr;
            if (getline(ss, amountStr, ',') && getline(ss, percentageStr, ',')) {
                Expense expense;
                expense.amount = stof(amountStr);
                expense.percentage = stof(percentageStr);
                spendings.push_back(expense);
            }
        }
        file.close();
    }
    else {
        cerr << "Error: Could not open lastSpendings.csv\n";
    }
    return spendings;
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

    const Rectangle dashboardButton = { 150, 970, 140, 75 };
    const Rectangle budgetButton = { 380, 970, 140, 75 };
    const Rectangle statisticsButton = { 610, 970, 140, 75 };

    USER_DATA userData = loadUserData();

    Texture2D manBigSize = LoadTexture("../images/m.png");
    Texture2D womanBigSize = LoadTexture("../images/w.png");
    Texture2D dashboardPhoto = LoadTexture("../images/dashboardImg.png");
    Texture2D background = LoadTexture("../images/greybackground.png");
    Texture2D statisticsPhoto = LoadTexture("../images/statistics.png");
    Texture2D budgetPhoto = LoadTexture("../images/budget.png");

    int newWidth = manBigSize.width / 2 + 30;
    int newHeight = manBigSize.height / 2;

    Image manImage = LoadImage("../images/m.png");
    Image womanImage = LoadImage("../images/w.png");
    Image dashboardImage = LoadImage("../images/dashboardImg.png");
    Image statisticsImage = LoadImage("../images/statistics.png");
    Image budgetImage = LoadImage("../images/budget.png");

    ImageResize(&manImage, newWidth - 15, newHeight);
    ImageResize(&womanImage, newWidth, newHeight);
    ImageResize(&dashboardImage, newWidth - 30, newHeight - 37);
    ImageResize(&statisticsImage, newWidth - 50, newHeight - 85);
    ImageResize(&budgetImage, newWidth - 30, newHeight - 70);

    Texture2D man = LoadTextureFromImage(manImage);
    Texture2D woman = LoadTextureFromImage(womanImage);
    Texture2D dashboardIcon = LoadTextureFromImage(dashboardImage);
    Texture2D statisticsIcon = LoadTextureFromImage(statisticsImage);
    Texture2D budgetIcon = LoadTextureFromImage(budgetImage);

    Validate validator;

    const Rectangle picToProfile = { GetScreenWidth() / 2 + 250, GetScreenHeight() / 2 - 500, man.width, man.height };
    const Rectangle exitButton = { GetScreenWidth() / 2 + 380, GetScreenHeight() / 2 - 520,50,50 };

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
                if ((key >= 32) && (key <= 125) && (wageInputLetterCount < 25)) 
                {
                    wageInput[wageInputLetterCount] = (char)key;
                    wageInput[wageInputLetterCount + 1] = '\0';
                    wageInputLetterCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) 
            {
                wageInputLetterCount--;
                if (wageInputLetterCount < 0) wageInputLetterCount = 0;
                wageInput[wageInputLetterCount] = '\0';
            }
        }

        vector<Expense> lastSpendings = loadLastSpendings();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(background, 0, 0, RAYWHITE);

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

        if (mouseOnWageInputBox && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            try
            {
                userData.balance += stof(wageInput);  // Update balance with wage input
                updateBalance(userData.balance);  // Save new balance to CSV
            }

            catch (const invalid_argument& e)
            {
                cerr << "Error: Invalid input for wage\n";
            }
        }

        int yPosition = 650;

        DrawText("Last spendings", 100, 600, 40, BLUE);
        DrawText("Housing", 100, 650, 30, BLACK);
        DrawText("Food", 100, 700, 30, BLACK);
        DrawText("Health", 100, 750, 30, BLACK);
        DrawText("Gifts", 100, 800, 30, BLACK);

        for (const Expense& expense : lastSpendings) {
            DrawText(TextFormat("$%.2f (%.2f%%)", expense.amount, expense.percentage), 125, yPosition, 20, DARKBLUE);
            yPosition += 50;
        }


        // Navigation buttons
        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(dashboardIcon, dashboardButton.x + 17, dashboardButton.y -5, RAYWHITE);
        if (isMouseOverDashboardButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ;
        }

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(budgetIcon, budgetButton.x + 17, budgetButton.y + 10, RAYWHITE);
        if (isMouseOverBudgetButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            budget();
        }

        bool isMouseOverStatisticsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatisticsButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(statisticsIcon, statisticsButton.x + 30, statisticsButton.y + 15, RAYWHITE);
        if (isMouseOverStatisticsButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            statistics();
        }

        bool isMouseOverExitButton = CheckCollisionPointRec(mousePosition, exitButton);
        DrawRectangleRounded(exitButton, 10, int(2), (isMouseOverExitButton ? RED : DARKGRAY));
        DrawText("X", exitButton.x +18, exitButton.y+15, 25, BLACK);
        if (isMouseOverExitButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
        }


        EndDrawing();
    }

    UnloadTexture(manBigSize);
    UnloadTexture(womanBigSize);
    UnloadTexture(man);
    UnloadTexture(woman);
    UnloadTexture(background);
    UnloadTexture(dashboardPhoto);
    UnloadTexture(statisticsPhoto);
    UnloadTexture(budgetPhoto);
}
