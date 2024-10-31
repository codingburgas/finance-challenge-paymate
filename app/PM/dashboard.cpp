#include "dashboard.h"
#include "budget.h"
#include "statistics.h"
#include "profile.h"
#include "accessData.h"
#include "validation.h"


struct USER_DATA {
    float monthlyIncome;
    float expenses;
    float balance;
    float growthRate;
};

void generateUserData()
{
    string monthlyIncome = "0";
    string expenses = "0";
    string balance = "0";
    string growthRate = "0";
    rapidcsv::Document accountData("../data/accountData.csv");
    int rowCount = accountData.GetRowCount();
    accountData.SetRow<string>(rowCount, { (string)currentUser, monthlyIncome, expenses, balance, growthRate });
    accountData.Save("../data/accountData.csv");
}

bool userDataExist()
{
    rapidcsv::Document accountData("../data/accountData.csv");
    vector<string> users = accountData.GetColumn<string>("username");
    return find(users.begin(), users.end(), (string)currentUser) != users.end();
}

USER_DATA loadUserData()
{
    if (!userDataExist())
    {
        generateUserData();
    }

    USER_DATA userData;
    rapidcsv::Document accountData("../data/accountData.csv");

    // Check if the row exists for `currentUser` and handle exceptions
    try {
        vector<string> data = accountData.GetRow<string>((string)currentUser);

        if (data.size() >= 5) {  // Ensure there are enough columns
            userData.monthlyIncome = stof(data[1]);
            userData.expenses = stof(data[2]);
            userData.balance = stof(data[3]);
            userData.growthRate = stof(data[4]);
        }
        else {
            userData.monthlyIncome = 0.0f;
            userData.expenses = 0.0f;
            userData.balance = 0.0f;
            userData.growthRate = 0.0f;
        }
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << '\n';
        // Set default values or handle error accordingly
        userData.monthlyIncome = 0.0f;
        userData.expenses = 0.0f;
        userData.balance = 0.0f;
        userData.growthRate = 0.0f;
    }

    return userData;
}

void dashboard()
{
    const int screenWidth = 900;
    const int screenHeight = 1080;

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
        DrawText("Marhaba, 6efa na Relefa", 50, 50, 20, DARKGRAY);
        DrawText("Good Afternoon", 50, 80, 18, GRAY);

        DrawText("Kesh Money", 50, 140, 18, BLUE);
        DrawText(TextFormat("$%.2f", userData.balance), 50, 170, 40, BLACK);
        DrawText("Overview", 50, 220, 16, LIGHTGRAY);

        DrawText(TextFormat("+ %.1f%%", userData.growthRate), 300, 170, 20, GREEN);

        Rectangle incomeBox = { 50, 300, 300, 100 };
        Rectangle expenseBox = { 380, 300, 300, 100 };

        DrawRectangleRounded(incomeBox, 0.2, 10, DARKBLUE);
        DrawText("Income", incomeBox.x + 20, incomeBox.y + 10, 20, RAYWHITE);
        DrawText(TextFormat("$%.2f", userData.monthlyIncome), incomeBox.x + 20, incomeBox.y + 40, 25, RAYWHITE);

        DrawRectangleRounded(expenseBox, 0.2, 10, RED);
        DrawText("Expenses", expenseBox.x + 20, expenseBox.y + 10, 20, RAYWHITE);
        DrawText(TextFormat("$%.2f", userData.expenses), expenseBox.x + 20, expenseBox.y + 40, 25, RAYWHITE);

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

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, budgetButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            budget();
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