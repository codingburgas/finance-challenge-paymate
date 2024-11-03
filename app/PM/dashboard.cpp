#include "dashboard.h"
#include "budget.h"
#include "statistics.h"
#include "profile.h"
#include "accessData.h"
#include "validation.h"

// Structure to hold user financial data.
struct USER_DATA
{
    float monthlyIncome;
    float savings;
    float balance;
};

// Function to load user data from a CSV file.
void generateUserData()
{
    string monthlyIncome = "0";
    string savings = "0";
    string balance = "0";
    string growthRates = "0";

    rapidcsv::Document accountData("../data/accountData.csv");

    int rowCount = accountData.GetRowCount();

    accountData.SetRow<string>(rowCount, { (string)currentUser, monthlyIncome, savings, balance, growthRates });
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

    catch (const out_of_range& e)
    {
        cerr << "Error: " << e.what() << '\n';

        // Set default values or handle error accordingly
        userData.monthlyIncome = 0.0f;
        userData.savings = 0.0f;
        userData.balance = 0.0f;
    }

    return userData;
}


// Function to update the balance for the current user in the CSV file.
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


// Function to load the last spendings from a CSV file. 
vector<float> loadLastSpendings()
{
    ifstream file("../data/diagramData.csv");

    // Vector to hold expense data.
    vector<float> expenses;

    // Check if file opened successfully
    if (!file.is_open())
    {
        return {};  // Return empty data if file can't be opened
    }
    string line;

    while (getline(file, line)) // Loop to read all lines in the file
    {
        istringstream ss(line);
        string value;

        if (line.substr(0, line.find(',')) != currentUser)
        {
            cout << line << endl;
            continue;
        }

        while (getline(ss, value, ',')) // Loop to read each comma-separated value
        {
            try
            {
                float expense = stof(value);
                expenses.push_back(expense);
            }

            catch (const invalid_argument&)
            {
                expenses.clear();  // Clear data on error and skip this line
                continue;
            }
        }
    }
    file.close();

    // Ensure we have exactly 4 expense entries, otherwise log an error
    if (expenses.size() != 4)
    {
        expenses.clear();  // Clear data if size is not as expected
    }

    return expenses;
}

// Function to update the savings for the current user in the CSV file.
void updateSavings(float newSavings)
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
        accountData.SetCell("savings", userRow, to_string(newSavings));
        accountData.Save("../data/accountData.csv");
    }
}

// Variables for wage input box
char wageInput[25] = "\0";
Rectangle wageInputBox = { 100, 600, 300, 80 };
bool mouseOnWageInputBox = false;
int wageInputLetterCount = 0;

// Variables for savings input box
char savingsInput[25] = "\0";
Rectangle savingsInputBox = { 100, 750, 300, 80 };
bool mouseOnSavingsInputBox = false;
int savingsInputLetterCount = 0;

// Function to display the dashboard.
void dashboard()
{
    Font font = LoadFont("../font/font.ttf");

    const int screenWidth = 900;
    const int screenHeight = 1080;

    Rectangle balanceBox = { 100, 220, 300, 250 };
    Rectangle savingsBox = { 500, 220, 300, 250 };

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

    // Load the last spendings data.
    vector<float> lastSpendings = loadLastSpendings();

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();

        SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (CheckCollisionPointRec(mousePosition, wageInputBox)) mouseOnWageInputBox = true;
        else mouseOnWageInputBox = false;

        if (mouseOnWageInputBox)
        {
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

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the background texture.
        DrawTexture(background, 0, 0, RAYWHITE);

        // Draw the black rectangle at the bottom of the screen.
        DrawRectangle(0, 930, 900, 200, BLACK);

        // Draw user profile picture based on gender.
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

        DrawTextEx(font, "Hello, ", Vector2{ (float)100, (float)80 }, 80, 3, BLACK);
        DrawTextEx(font, currentUser, Vector2{ (float)300,(float)80 }, 80, 3, BLACK);

        // Display balance
        DrawRectangleRounded(balanceBox, 0.2f, 10, DARKBLUE);
        DrawTextEx(font, "Balance", Vector2{ (float)balanceBox.x + 57, (float)balanceBox.y + 50 }, 60, 2, RAYWHITE);
        DrawText(TextFormat("$%.2f", userData.balance), balanceBox.x + 35, balanceBox.y + 130, 50, RAYWHITE);

        // Display savings.
        DrawRectangleRounded(savingsBox, 0.2, 10, RED);
        DrawTextEx(font, "Savings", Vector2{ (float)savingsBox.x + 67, (float)savingsBox.y + 50 }, 60, 2, RAYWHITE);
        DrawText(TextFormat("$%.2f", userData.savings), savingsBox.x + 35, savingsBox.y + 130, 50, RAYWHITE);

        // Wage input box
        DrawRectangleRec(wageInputBox, LIGHTGRAY);
        DrawTextEx(font, "Enter Wage:", Vector2{ (float)wageInputBox.x,  (float)wageInputBox.y - 50 }, 30, 2, DARKGRAY);
        DrawTextEx(font, wageInput, Vector2{ (float)wageInputBox.x + 5, (float)wageInputBox.y + 15 }, 50, 2, BLACK);

        // Update balance with wage input if the input box is clicked.
        if (mouseOnWageInputBox && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            userData.balance += stof(wageInput);  // Update balance with wage input
            updateBalance(userData.balance);  // Save new balance to CSV
        }

        // Savings input box.
        DrawRectangleRec(savingsInputBox, LIGHTGRAY);
        DrawTextEx(font, "Enter Savings:", Vector2{ (float)savingsInputBox.x, (float)savingsInputBox.y - 50 }, 30, 2, DARKGRAY);
        DrawTextEx(font, savingsInput, Vector2{ (float)savingsInputBox.x + 5, (float)savingsInputBox.y + 15 }, 50, 2, BLACK);

        // Check if the mouse is over the savings input box.
        if (CheckCollisionPointRec(mousePosition, savingsInputBox)) mouseOnSavingsInputBox = true;
        else mouseOnSavingsInputBox = false;

        // Handle savings input.
        if (mouseOnSavingsInputBox)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (savingsInputLetterCount < 25))
                {
                    savingsInput[savingsInputLetterCount] = (char)key;
                    savingsInput[savingsInputLetterCount + 1] = '\0';
                    savingsInputLetterCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                savingsInputLetterCount--;
                if (savingsInputLetterCount < 0) savingsInputLetterCount = 0;
                savingsInput[savingsInputLetterCount] = '\0';
            }
        }

        // Update savings and balance if the savings input box is clicked.
        if (mouseOnSavingsInputBox && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            float savingsAmount = stof(savingsInput);
            userData.balance -= savingsAmount;
            updateBalance(userData.balance);
            userData.savings += stof(savingsInput);
            updateSavings(userData.savings);

        }

        int yPosition = 650;

        // Display last spendings.
        DrawTextEx(font, "Last spendings", Vector2{ (float)500,  (float)550 }, 50, 2, BLUE);
        DrawTextEx(font, "Housing", Vector2{ (float)500,  (float)650 }, 40, 2, BLACK);
        DrawTextEx(font, "Food", Vector2{ (float)500,  (float)700 }, 40, 2, BLACK);
        DrawTextEx(font, "Health", Vector2{ (float)500,  (float)750 }, 40, 2, BLACK);
        DrawTextEx(font, "Gifts", Vector2{ (float)500,  (float)800 }, 40, 2, BLACK);

        for (float expense : lastSpendings)
        {
            DrawTextEx(font, TextFormat("- $% .2f", expense), Vector2{ (float)650, (float)yPosition}, 35, 2, RED);
            yPosition += 50;
        }


        // Navigation buttons
        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(dashboardIcon, dashboardButton.x + 17, dashboardButton.y - 5, RAYWHITE);
        if (isMouseOverDashboardButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ;
        }

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(budgetIcon, budgetButton.x + 17, budgetButton.y + 10, RAYWHITE);
        if (isMouseOverBudgetButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            budget();
        }

        bool isMouseOverStatisticsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatisticsButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(statisticsIcon, statisticsButton.x + 30, statisticsButton.y + 15, RAYWHITE);
        if (isMouseOverStatisticsButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            statistics();
        }

        bool isMouseOverExitButton = CheckCollisionPointRec(mousePosition, exitButton);
        DrawRectangleRounded(exitButton, 10, int(2), (isMouseOverExitButton ? RED : DARKGRAY));
        DrawTextEx(font, "X", Vector2{ (float)exitButton.x + 18,  (float)exitButton.y + 15 }, 25, 10, BLACK);
        if (isMouseOverExitButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            CloseWindow();
        }


        EndDrawing();
    }

    //Unload all textures
    UnloadTexture(manBigSize);
    UnloadTexture(womanBigSize);
    UnloadTexture(man);
    UnloadTexture(woman);
    UnloadTexture(background);
    UnloadTexture(dashboardPhoto);
    UnloadTexture(statisticsPhoto);
    UnloadTexture(budgetPhoto);
    UnloadTexture(dashboardIcon);
    UnloadTexture(statisticsIcon);
    UnloadTexture(budgetIcon);
    //Unload font
    UnloadFont(font);
}