#include "statistics.h"
#include "dashboard.h"
#include "budget.h"
#include "profile.h"
#include "accessData.h"
#include "validation.h"

struct USER_DATA
{
    vector<float> expenses;  // Holds the four expense values
};

// Load diagram data from CSV
USER_DATA loadDiagramData()
{
    USER_DATA userData;
    ifstream file("../data/diagramData.csv");

    // Check if file opened successfully
    if (!file.is_open())
    {
        cerr << "Error: Could not open diagramData.csv\n";
        return userData;  // Return empty data if file can't be opened
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
            cout << value << endl;  // Display value (for debugging)

            try 
            {
                float expense = stof(value);  // Convert string to float
                userData.expenses.push_back(expense);
            }

            catch (const invalid_argument&)
            {
                cerr << "Error: Invalid data in diagramData.csv\n";
                userData.expenses.clear();  // Clear data on error and skip this line
                continue;
            }
        }
    }
    file.close();

    // Ensure we have exactly 4 expense entries, otherwise log an error
    if (userData.expenses.size() != 4)
    {
        cerr << "Error: diagramData.csv does not contain 4 expense values.\n";
        userData.expenses.clear();  // Clear data if size is not as expected
    }

    return userData;
}

extern USER_DATA loadUserData();

void statistics()
{
    const int screenWidth = 900;
    const int screenHeight = 1080;

    const Rectangle dashboardButton = { 150, 970, 140, 75 };
    const Rectangle budgetButton = { 380, 970, 140, 75 };
    const Rectangle statisticsButton = { 610, 970, 140, 75 };

    Texture2D manBigSize = LoadTexture("../images/m.png");
    Texture2D womanBigSize = LoadTexture("../images/w.png");

    int newWidth = manBigSize.width / 2 + 30;
    int newHeight = manBigSize.height / 2;

    Image manImage = LoadImage("../images/m.png");
    Image womanImage = LoadImage("../images/w.png");

    ImageResize(&manImage, newWidth - 15, newHeight);
    ImageResize(&womanImage, newWidth, newHeight);

    Texture2D man = LoadTextureFromImage(manImage);
    Texture2D woman = LoadTextureFromImage(womanImage);

    Validate validator;
   
    const Rectangle picToProfile = { GetScreenWidth() / 2 + 250, GetScreenHeight() / 2 - 500, man.width, man.height };

    SetTargetFPS(60);

    const int barWidth = 130;
    const int maxBarHeight = 100;
    const int xOffset = 150;
    const int yOffset = screenHeight - 600;

    USER_DATA userData = loadDiagramData();  // Load expense data for diagram

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();

        BeginDrawing();

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

        DrawText(currentUser, 80, 80, 40, DARKGRAY);
        DrawText("'s ", 280, 80, 40, DARKGRAY);
        DrawText("Monthly Expenses", 80, 140, 40, DARKGRAY);

        if (userData.expenses.empty())
        {
            DrawText("No data available for diagram.", xOffset, yOffset - 100, 20, RED);
        }

        else
        {
            // Draw the bar chart with expenses
            for (size_t i = 0; i < userData.expenses.size(); i++) 
            {
                int barHeight = static_cast<int>((userData.expenses[i] * maxBarHeight) / 100);  // Scale to fit
                DrawRectangle(xOffset + i * (barWidth + 10), yOffset - barHeight, barWidth, barHeight, BLUE);
            }

            DrawText("Housing", xOffset + 27, yOffset + 10, 20, DARKGRAY);
            DrawText("Food", (xOffset * 2) + 30, yOffset + 10, 20, DARKGRAY);
            DrawText("Health", (xOffset * 3) + 17, yOffset + 10, 20, DARKGRAY);
            DrawText("Gifts", (xOffset * 4) + 10, yOffset + 10, 20, DARKGRAY);
        }

        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, dashboardButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            dashboard();
        }

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, budgetButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            budget();
        }

        bool isMouseOverStatistricsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatistricsButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, statisticsButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            ;
        }

        EndDrawing();
    }

    UnloadTexture(manBigSize);
    UnloadTexture(womanBigSize);
    UnloadTexture(man);
    UnloadTexture(woman);
}