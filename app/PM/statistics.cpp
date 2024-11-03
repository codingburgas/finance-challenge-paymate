#include "statistics.h"
#include "dashboard.h"
#include "budget.h"
#include "profile.h"
#include "accessData.h"
#include "validation.h"

// Holds the four expense values
struct USER_DATA
{
    vector<float> expenses; 
};

// Load diagram data from CSV
USER_DATA loadDiagramData()
{
    USER_DATA userData;
    ifstream file("../data/diagramData.csv");

    // Check if file opened successfully
    if (!file.is_open())
    {
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
            try 
            {
                float expense = stof(value);  
                userData.expenses.push_back(expense);
            }

            catch (const invalid_argument&)
            {
                userData.expenses.clear();  // Clear data on error and skip this line
                continue;
            }
        }
    }
    file.close();

    // Ensure we have exactly 4 expense entries, otherwise log an error
    if (userData.expenses.size() != 4)
    {
        userData.expenses.clear();  // Clear data if size is not as expected
    }

    return userData;
}

extern USER_DATA loadUserData();

void statistics()
{
    Font font = LoadFont("../font/font.ttf");

    float maxValue = -9999.99;
    int maxIndex = -1;

    const int screenWidth = 900;
    const int screenHeight = 1080;

    const Rectangle dashboardButton = { 150, 970, 140, 75 };
    const Rectangle budgetButton = { 380, 970, 140, 75 };
    const Rectangle statisticsButton = { 610, 970, 140, 75 };

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

    // Resize images for the interface
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

    const int barWidth = 130;
    const int maxBarHeight = 100;
    const int xOffset = 150;
    const int yOffset = screenHeight - 600;

    // Load expense data for diagram
    USER_DATA userData = loadDiagramData();  

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawTexture(background, 0, 0, RAYWHITE);
        DrawRectangle(0, 930, 900, 200, BLACK);

        // Draw user profile picture based on gender
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

        // Display user name and expenses title
        DrawTextEx(font, currentUser, Vector2{ (float)80, (float)80 }, 50, 10, DARKGRAY);
        DrawTextEx(font, "'s ", Vector2{ (float)330, (float)80 }, 50, 10, DARKGRAY);
        DrawTextEx(font, "Monthly Expenses", Vector2{ (float)80, (float)140 }, 50, 10, DARKGRAY);

        if (userData.expenses.empty())
        {
            DrawTextEx(font, "No data available for diagram.", Vector2{ (float)xOffset, (float)yOffset - 100 }, 25, 2, RED);
        }

        else
        {
            // Draw the bar chart with expenses
            for (size_t i = 0; i < userData.expenses.size(); i++)
            {
                int barHeight = static_cast<int>((userData.expenses[i] * maxBarHeight) / 100);  // Scale to fit
                DrawRectangle(xOffset + i * (barWidth + 10), yOffset - barHeight, barWidth, barHeight, RED);
            }

            // Label each expense category
            DrawTextEx(font, "Housing", Vector2{ (float)xOffset + 27, (float)yOffset + 10}, 25, 2, DARKGRAY);
            DrawTextEx(font, "Food", Vector2{ (float)(xOffset * 2) + 30, (float)yOffset + 10 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "Health", Vector2{ (float)(xOffset * 3) + 15, (float)yOffset + 10}, 25, 2, DARKGRAY);
            DrawTextEx(font, "Gifts", Vector2{ (float)(xOffset * 4) + 10, (float)yOffset + 10}, 25, 2, DARKGRAY);
        }

        // Find the max value and its index in userData.expenses
        for (size_t i = 0; i < userData.expenses.size(); i++) 
        {
            if (userData.expenses[i] > maxValue) 
            {
                maxValue = userData.expenses[i];
                maxIndex = static_cast<int>(i);
            }
        }

        // Display a specific message based on which expense is the highest
        switch (maxIndex) 
        {
        case 0: //Highest expense in housing
            DrawTextEx(font, "Highest expense: Housing", Vector2{ (float)xOffset - 70, (float)yOffset + 80 }, 30, 2, BLUE);

            //First tip
            DrawTextEx(font, "1. Evaluate Subscription Plans and Utility Usage: Review your internet,", Vector2{ (float)xOffset - 50, (float)yOffset + 130 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "phone, and utility plans to see if there are cheaper options that fit your", Vector2{ (float)xOffset - 50, (float)yOffset + 160 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "usage better. Adjust settings to use less electricity and water (e.g.,", Vector2{ (float)xOffset - 50, (float)yOffset + 190 }, 25, 2, DARKGRAY);
            DrawTextEx(font, " energy-efficient bulbs, shorter showers) to lower monthly bills. ", Vector2{ (float)xOffset - 50, (float)yOffset + 220 }, 25, 2, DARKGRAY);
            
            //Second tip
            DrawTextEx(font, "2. Consider a Budget or Downsize: If rent or mortgage is taking up too ", Vector2{ (float)xOffset - 50, (float)yOffset + 270 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "much of your income, consider creating a detailed budget to identify ", Vector2{ (float)xOffset - 50, (float)yOffset + 300 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "areas for saving, or explore downsizing to a smaller, more affordable ", Vector2{ (float)xOffset - 50, (float)yOffset + 330 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "space if possible.", Vector2{ (float)xOffset - 50, (float)yOffset + 360 }, 25, 2, DARKGRAY);

            break;

        case 1: //Highest expense in food
            DrawTextEx(font, "Highest expense: Food", Vector2{ (float)xOffset - 70, (float)yOffset + 80 }, 25, 2, BLUE);

            //First tip
            DrawTextEx(font, "1. Plan and Track Purchases: Set a weekly or monthly budget for", Vector2{ (float)xOffset - 50, (float)yOffset + 130 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "each of these categories. Use an app or keep a simple list to track ", Vector2{ (float)xOffset - 50, (float)yOffset + 160 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "your spending, so you can make adjustments before going over budget.", Vector2{ (float)xOffset - 50, (float)yOffset + 190 }, 25, 2, DARKGRAY);

            //Second tip
            DrawTextEx(font, "2. Limit Convenience Purchases: For non-essential items like coffee,  ", Vector2{ (float)xOffset - 50,(float)yOffset + 240 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "delivery, and eating out, try setting a rule to limit these purchases to", Vector2{ (float)xOffset - 50, (float)yOffset + 270 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "once a week. Prep meals and snacks at home to save on both groceries", Vector2{ (float)xOffset - 50, (float)yOffset + 300 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "and takeout!", Vector2{ (float)xOffset - 50, (float)yOffset + 330 }, 25, 2, DARKGRAY);

            break;

        case 2: //Highest expense in health
            DrawTextEx(font, "Highest expense: Health", Vector2{ (float)xOffset - 70, (float)yOffset + 80 }, 25, 2, BLUE);

            //First tip
            DrawTextEx(font, "1. Create a Budget and Track Spending: Establish a budget specifically", Vector2{ (float)xOffset - 50, (float)yOffset + 130 }, 25, 2, DARKGRAY);
            DrawTextEx(font, " for health and wellness expenses. Monitor your spending closely ", Vector2{ (float)xOffset - 50, (float)yOffset + 160 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "to identify areas where you might reduce costs, such as opting for", Vector2{ (float)xOffset - 50, (float)yOffset + 190 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "group fitness classes instead of personal training.", Vector2{ (float)xOffset - 50, (float)yOffset + 220 }, 25, 2, DARKGRAY);

            //Second tip
            DrawTextEx(font, "2. Explore Alternatives and Resources: Research alternative options", Vector2{ (float)xOffset - 50, (float)yOffset + 270 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "that might be more cost-effective. For example, consider community", Vector2{ (float)xOffset - 50, (float)yOffset + 300 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "programs for fitness or mental health services, seek out free or ", Vector2{ (float)xOffset - 50, (float)yOffset + 330 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "low-cost wellness workshops, or utilize online resources for workouts", Vector2{ (float)xOffset - 50, (float)yOffset + 360 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "and therapy.", Vector2{ (float)xOffset - 50, (float)yOffset + 390 }, 20, 2, DARKGRAY);

            break;

        case 3: //Highest expense in gifts
            DrawTextEx(font, "Highest expense: Gifts", Vector2{ (float)xOffset - 70, (float)yOffset + 80 }, 30, 2, BLUE);

            //First tip
            DrawTextEx(font, "1. Set a Budget: Determine a clear budget for gifts, donations, ", Vector2{ (float)xOffset - 50, (float)yOffset + 130 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "and celebrations each month. Allocate specific amounts for each", Vector2{ (float)xOffset - 50, (float)yOffset + 160 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "category, and stick to these limits to avoid overspending.", Vector2{ (float)xOffset - 50, (float)yOffset + 190 }, 25, 2, DARKGRAY);

            //Second tip
            DrawTextEx(font, "2. Prioritize and Plan: Before making purchases, prioritize which", Vector2{ (float)xOffset - 50, (float)yOffset + 240 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "gifts or events are most important. Plan ahead for larger expenses", Vector2{ (float)xOffset - 50, (float)yOffset + 270 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "and consider alternatives like homemade gifts or smaller gatherings", Vector2{ (float)xOffset - 50, (float)yOffset + 300 }, 25, 2, DARKGRAY);
            DrawTextEx(font, "to save money without sacrificing thoughtfulness.", Vector2{ (float)xOffset - 50, (float)yOffset + 330 }, 25, 2, DARKGRAY);

            break;

        default:
            DrawTextEx(font, "Error: Unable to determine highest expense", Vector2{ (float)xOffset - 70, (float)yOffset + 120 }, 25, 2, RED);

            break;
        }

        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(dashboardIcon, dashboardButton.x + 17, dashboardButton.y - 5, RAYWHITE);
        if (isMouseOverDashboardButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            dashboard();
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
            ;
        }

        bool isMouseOverExitButton = CheckCollisionPointRec(mousePosition, exitButton);
        DrawRectangleRounded(exitButton, 10, int(2), (isMouseOverExitButton ? RED : DARKGRAY));
        DrawTextEx(font, "X", Vector2{ (float)exitButton.x + 18, (float)exitButton.y + 15 }, 25, 10, BLACK);
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