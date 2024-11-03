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

    USER_DATA userData = loadDiagramData();  // Load expense data for diagram

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();

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
                DrawRectangle(xOffset + i * (barWidth + 10), yOffset - barHeight, barWidth, barHeight, RED);
            }

            DrawText("Housing", xOffset + 27, yOffset + 10, 20, DARKGRAY);
            DrawText("Food", (xOffset * 2) + 30, yOffset + 10, 20, DARKGRAY);
            DrawText("Health", (xOffset * 3) + 17, yOffset + 10, 20, DARKGRAY);
            DrawText("Gifts", (xOffset * 4) + 10, yOffset + 10, 20, DARKGRAY);
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
            DrawText("Highest expense: Housing", xOffset - 70, yOffset + 80, 20, BLUE);

            //First tip
            DrawText("1. Evaluate Subscription Plans and Utility Usage: Review your internet,", xOffset - 50, yOffset + 130, 20, DARKGRAY);
            DrawText("phone, and utility plans to see if there are cheaper options that fit your", xOffset - 50, yOffset + 160, 20, DARKGRAY);
            DrawText("usage better. Adjust settings to use less electricity and water (e.g.,", xOffset - 50, yOffset + 190, 20, DARKGRAY);
            DrawText(" energy-efficient bulbs, shorter showers) to lower monthly bills. ", xOffset - 50, yOffset + 220, 20, DARKGRAY);
            
            //Second tip
            DrawText("2. Consider a Budget or Downsize: If rent or mortgage is taking up too ", xOffset - 50, yOffset + 270, 20, DARKGRAY);
            DrawText("much of your income, consider creating a detailed budget to identify areas", xOffset - 50, yOffset + 300, 20, DARKGRAY);
            DrawText("for saving, or explore downsizing to a smaller, more affordable space if", xOffset - 50, yOffset + 330, 20, DARKGRAY);
            DrawText("possible.", xOffset - 50, yOffset + 360, 20, DARKGRAY);

            break;

        case 1: //Highest expense in ood
            DrawText("Highest expense: Food", xOffset - 70, yOffset + 80, 20, BLUE);

            //First tip
            DrawText("1. Plan and Track Purchases: Set a weekly or monthly budget for", xOffset - 50, yOffset + 130, 20, DARKGRAY);
            DrawText("each of these categories. Use an app or keep a simple list to track ", xOffset - 50, yOffset + 160, 20, DARKGRAY);
            DrawText("your spending, so you can make adjustments before going over budget.", xOffset - 50, yOffset + 190, 20, DARKGRAY);

            //Second tip
            DrawText("2. Limit Convenience Purchases: For non-essential items like coffee,  ", xOffset - 50, yOffset + 240, 20, DARKGRAY);
            DrawText("delivery, and eating out, try setting a rule to limit these purchases to", xOffset - 50, yOffset + 270, 20, DARKGRAY);
            DrawText("once a week. Prep meals and snacks at home to save on both groceries", xOffset - 50, yOffset + 300, 20, DARKGRAY);
            DrawText("and takeout!", xOffset - 50, yOffset + 330, 20, DARKGRAY);

            break;

        case 2: //Highest expense in health
            DrawText("Highest expense: Health", xOffset - 70, yOffset + 80, 20, BLUE);

            //First tip
            DrawText("1. Create a Budget and Track Spending: Establish a budget specifically", xOffset - 50, yOffset + 130, 20, DARKGRAY);
            DrawText(" for health and wellness expenses. Monitor your spending closely ", xOffset - 50, yOffset + 160, 20, DARKGRAY);
            DrawText("to identify areas where you might reduce costs, such as opting for", xOffset - 50, yOffset + 190, 20, DARKGRAY);
            DrawText("group fitness classes instead of personal training.", xOffset - 50, yOffset + 220, 20, DARKGRAY);

            //Second tip
            DrawText("2. Explore Alternatives and Resources: Research alternative options", xOffset - 50, yOffset + 270, 20, DARKGRAY);
            DrawText("that might be more cost-effective. For example, consider community", xOffset - 50, yOffset + 300, 20, DARKGRAY);
            DrawText("programs for fitness or mental health services, seek out free or ", xOffset - 50, yOffset + 330, 20, DARKGRAY);
            DrawText("low-cost wellness workshops, or utilize online resources for workouts", xOffset - 50, yOffset + 360, 20, DARKGRAY);
            DrawText("and therapy.", xOffset - 50, yOffset + 390, 20, DARKGRAY);

            break;

        case 3: //Highest expense in gifts
            DrawText("Highest expense: Gifts", xOffset - 70, yOffset + 80, 20, BLUE);

            //First tip
            DrawText("1. Set a Budget: Determine a clear budget for gifts, donations, ", xOffset - 50, yOffset + 130, 20, DARKGRAY);
            DrawText("and celebrations each month. Allocate specific amounts for each", xOffset - 50, yOffset + 160, 20, DARKGRAY);
            DrawText("category, and stick to these limits to avoid overspending.", xOffset - 50, yOffset + 190, 20, DARKGRAY);

            //Second tip
            DrawText("2. Prioritize and Plan: Before making purchases, prioritize which", xOffset - 50, yOffset + 240, 20, DARKGRAY);
            DrawText("gifts or events are most important. Plan ahead for larger expenses", xOffset - 50, yOffset + 270, 20, DARKGRAY);
            DrawText("and consider alternatives like homemade gifts or smaller gatherings to ", xOffset - 50, yOffset + 300, 20, DARKGRAY);
            DrawText("save money without sacrificing thoughtfulness.", xOffset - 50, yOffset + 330, 20, DARKGRAY);

            break;

        default:
            DrawText("Error: Unable to determine highest expense", xOffset - 70, yOffset + 120, 20, RED);

            break;
        }

        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(dashboardIcon, dashboardButton.x + 17, dashboardButton.y - 5, RAYWHITE);
        if (isMouseOverDashboardButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dashboard;
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
            ;
        }

        bool isMouseOverExitButton = CheckCollisionPointRec(mousePosition, exitButton);
        DrawRectangleRounded(exitButton, 10, int(2), (isMouseOverExitButton ? RED : DARKGRAY));
        DrawText("X", exitButton.x + 18, exitButton.y + 15, 25, BLACK);
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