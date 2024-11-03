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

// Save expenses to diagramData.csv
void saveDiagramData(const char* username, const char* housing, const char* food, const char* health, const char* gifts) 
{
    string filePath = "../data/diagramData.csv";
    ifstream inputFile(filePath);
    vector<string> lines;
    
    bool userExists = false;

    // Read all lines into memory and look for the username
    if (inputFile.is_open())
    {
        string line;
       
        while (getline(inputFile, line)) 
        {
            istringstream lineStream(line);
            string existingUsername;
            getline(lineStream, existingUsername, ',');

            // Check if the username exists
            if (existingUsername == username) 
            {
                // Replace this line with the new data
                string updatedLine = string(username) + ',' + housing + ',' + food + ',' + health + ',' + gifts;
                lines.push_back(updatedLine);
                userExists = true;
            }

            else 
            {
                lines.push_back(line);  // Keep other lines as is
            }
        }

        inputFile.close();
    }

    else 
    {
        cerr << "Error: Could not open diagramData.csv\n";
        return;
    }

    // If the user was not found, append the new user data
    if (!userExists) 
    {
        string newLine = string(username) + ',' + housing + ',' + food + ',' + health + ',' + gifts;
        lines.push_back(newLine);
    }

    // Write all lines back to the file
    ofstream outputFile(filePath);
    if (outputFile.is_open()) 
    {
        for (const string& line : lines) 
        {
            outputFile << line << "\n";
        }
        outputFile.close();
    }

    else 
    {
        cerr << "Error: Could not open diagramData.csv for writing\n";
    }
}

extern USER_DATA generateUserData();
extern USER_DATA loadUserData();

extern bool userDataExist();

extern void updateBalance(float newBalance);

void updateDiagram(float newDiagram) 
{
    rapidcsv::Document diagramData("../data/diagramData.csv");

    vector<string> usernames = diagramData.GetColumn<string>("username");
    int userRow = -1;
    for (int i = 0; i < usernames.size(); i++) 
    {
        if (usernames[i] == currentUser) 
        {
            userRow = i;
            break;
        }
    }

    if (userRow != -1) {
        diagramData.SetCell("username", userRow, to_string(newDiagram));
        diagramData.Save("../data/diagramData.csv");
    }
}

void budget() {
    

    const int screenWidth = 900;
    const int screenHeight = 1080;
    Texture2D background = LoadTexture("../images/budgetpage.png");

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

    Rectangle inputBox1 = { 100, 400, 180, 45 };
    Rectangle inputBox2 = { 560, 400, 180, 45 };
    Rectangle inputBox3 = { 100, 780, 180, 45 };
    Rectangle inputBox4 = { 560, 780, 180, 45 };
    Rectangle submitDataButton = { 345, 840, 140, 75 };

    USER_DATA userData = loadUserData();  // Load the user's data

    DataAccess dataAccess;
    
    Texture2D manBigSize = LoadTexture("../images/m.png");
    Texture2D womanBigSize = LoadTexture("../images/w.png");
    Texture2D dashboardPhoto = LoadTexture("../images/dashboardImg.png");
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
    ImageResize(&dashboardImage, newWidth- 30, newHeight-37);
    ImageResize(&statisticsImage, newWidth - 50, newHeight -85);
    ImageResize(&budgetImage, newWidth- 30, newHeight - 70);

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

        BeginDrawing();

        // Draw the background texture first so that other elements appear on top
        DrawTexture(background, 0, 0, WHITE);

        // Check mouse position over input boxes
        mouseOnInputBox1 = CheckCollisionPointRec(mousePosition, inputBox1);
        mouseOnInputBox2 = CheckCollisionPointRec(mousePosition, inputBox2);
        mouseOnInputBox3 = CheckCollisionPointRec(mousePosition, inputBox3);
        mouseOnInputBox4 = CheckCollisionPointRec(mousePosition, inputBox4);

        // Set mouse cursor
        if (mouseOnInputBox1 || mouseOnInputBox2 || mouseOnInputBox3 || mouseOnInputBox4)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        }

        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // Handle input for each box
        if (mouseOnInputBox1)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (inputBox1LetterCount < 24))
                {
                    input1[inputBox1LetterCount++] = (char)key;
                    input1[inputBox1LetterCount] = '\0'; // Null-terminate the string
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && inputBox1LetterCount > 0)
            {
                input1[--inputBox1LetterCount] = '\0';
            }
        }

        if (mouseOnInputBox2)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (inputBox2LetterCount < 24))
                {
                    input2[inputBox2LetterCount++] = (char)key;
                    input2[inputBox2LetterCount] = '\0';
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) && inputBox2LetterCount > 0)
            {
                input2[--inputBox2LetterCount] = '\0';
            }
        }

        if (mouseOnInputBox3)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (inputBox3LetterCount < 24))
                {
                    input3[inputBox3LetterCount++] = (char)key;
                    input3[inputBox3LetterCount] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputBox3LetterCount > 0)
            {
                input3[--inputBox3LetterCount] = '\0';
            }
        }

        if (mouseOnInputBox4)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (inputBox4LetterCount < 24))
                {
                    input4[inputBox4LetterCount++] = (char)key;
                    input4[inputBox4LetterCount] = '\0';
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && inputBox4LetterCount > 0)
            {
                input4[--inputBox4LetterCount] = '\0';
            }
        }


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
        DrawRectangleRounded(submitDataButton, 0.2, 10, BLACK);
        DrawText("  Submit", submitDataButton.x + 15, submitDataButton.y + 20, 25, RAYWHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, submitDataButton))
        {
            try
            {
                // Save expenses to diagramData.csv
                saveDiagramData(currentUser, input1, input2, input3, input4);
                // Calculate total expenses and update balance
                float totalExpense = stof(input1) + stof(input2) + stof(input3) + stof(input4);
                userData.balance -= totalExpense;  // Deduct expenses from balance
                updateBalance(userData.balance);   // Save updated balance to CSV
            }

            catch (const invalid_argument& e)
            {
                cerr << "Error: Invalid input for expenses\n";
            }
        }

        // Draw navigation buttons
        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(dashboardIcon, dashboardButton.x + 17, dashboardButton.y - 5, RAYWHITE);
        if (isMouseOverDashboardButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dashboard();
        }

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(budgetIcon, budgetButton.x + 17, budgetButton.y + 10, RAYWHITE);
        if (isMouseOverBudgetButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ;
        }

        bool isMouseOverStatisticsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatisticsButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(statisticsIcon, statisticsButton.x + 30, statisticsButton.y + 15, RAYWHITE);
        if (isMouseOverStatisticsButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            statistics();
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
