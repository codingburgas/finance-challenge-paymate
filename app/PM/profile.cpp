#include "profile.h"
#include "dashboard.h"
#include "budget.h"
#include "statistics.h"
#include "validation.h"
#include "accessData.h"

bool editPassword(const string& username, const string& oldPass, const string& newPass) 
{
    ifstream file("../data/accounts.csv");
    vector<string> lines;
    string line, storedUsername, storedPassword, storedGender;
    bool passwordUpdated = false;
    Validate validator;

    if (!file.is_open())
    {
        cerr << "Error: Could not open accounts file." << endl;
        return false;
    }
   
    while (getline(file, line)) 
    {
        istringstream iss(line);
        if (getline(iss, storedUsername, ',') &&
            getline(iss, storedPassword, ',') &&
            getline(iss, storedGender))
        {
            if (storedUsername == username) 
            {
                // Verify if the old password matches
                if (storedPassword == oldPass) 
                {
                    // Update password
                    if (validator.containsUppercase(newPass) && validator.containsLowercase(newPass) && validator.containsDigit(newPass) && validator.containsSpecial(newPass))
                    {
                        storedPassword = newPass;
                        passwordUpdated = true;
                    }
                }

                else 
                {
                    
                    file.close();
                    return false;
                }
            }

            // Rebuild line with updated password if applicable
            ostringstream oss;
            oss << storedUsername << "," << storedPassword << "," << storedGender;
            lines.push_back(oss.str());
        }

        else 
        {
            lines.push_back(line); // Invalid line format, but keep as-is
        }
    }
    file.close();


    ofstream outFile("../data/accounts.csv", ios::trunc);
    if (!outFile.is_open()) 
    {
        return false;
    }

    for (const auto& l : lines) 
    {
        outFile << l << "\n";
    }
    outFile.close();

    return passwordUpdated;
}

void profile()
{
    const int screenWidth = 900;
    const int screenHeight = 1080;

    const Rectangle dashboardButton = { 150, 970, 140, 75 };
    const Rectangle budgetButton = { 380, 970, 140, 75 };
    const Rectangle statisticsButton = { 610, 970, 140, 75 };
   
    Texture2D profileTemplateMan = LoadTexture("../images/mprofile.png");
    Texture2D profileTemplateWoman = LoadTexture("../images/wprofile.png");

    int newWidth = profileTemplateMan.width / 8;
    int newHeight = profileTemplateMan.height / 8;

    Texture2D dashboardPhoto = LoadTexture("../images/dashboardImg.png");
    Texture2D statisticsPhoto = LoadTexture("../images/statistics.png");
    Texture2D budgetPhoto = LoadTexture("../images/budget.png");
   
    Image dashboardImage = LoadImage("../images/dashboardImg.png");
    Image statisticsImage = LoadImage("../images/statistics.png");
    Image budgetImage = LoadImage("../images/budget.png");

    ImageResize(&dashboardImage, newWidth - 5, newHeight - 55);
    ImageResize(&budgetImage, newWidth - 5, newHeight - 75);
    ImageResize(&statisticsImage, newWidth - 25, newHeight - 95);

    Texture2D dashboardIcon = LoadTextureFromImage(dashboardImage);
    Texture2D statisticsIcon = LoadTextureFromImage(statisticsImage);
    Texture2D budgetIcon = LoadTextureFromImage(budgetImage);

    Validate validator;

    const Rectangle exitButton = { GetScreenWidth() / 2 + 380, GetScreenHeight() / 2 - 520,50,50 };

    char oldPass[25] = "\0";
    char newPass[25] = "\0";

    int oldPassLetterCount = 0;
    int newPassLetterCount = 0;

    const Rectangle oldPassBox = { GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 140 , 550, 65 };
    const Rectangle newPassBox = { GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 260 , 550, 65 };

    bool mouseOnOldPass = false;
    bool mouseOnNewPass = false;

    const Rectangle editButton = { GetScreenWidth() / 2 + 250, GetScreenHeight() / 2 + 300, 100, 45 };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePosition = GetMousePosition();

        if (CheckCollisionPointRec(mousePosition, oldPassBox)) mouseOnOldPass = true;
        else mouseOnOldPass = false;

        if (CheckCollisionPointRec(mousePosition, newPassBox)) mouseOnNewPass = true;
        else mouseOnNewPass = false;

        if (mouseOnOldPass || mouseOnNewPass) SetMouseCursor(MOUSE_CURSOR_IBEAM);
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnOldPass) 
        {
            int key = GetCharPressed();

            while (key > 0) 
            {
                if ((key >= 32) && (key <= 125) && (oldPassLetterCount < 25)) 
                {
                    oldPass[oldPassLetterCount] = (char)key;
                    oldPass[oldPassLetterCount + 1] = '\0';
                    oldPassLetterCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) 
            {
                oldPassLetterCount--;
                if (oldPassLetterCount < 0) oldPassLetterCount = 0;
                oldPass[oldPassLetterCount] = '\0';
            }
        }

        if (mouseOnNewPass) 
        {
            int key = GetCharPressed();

            while (key > 0) 
            {
                if ((key >= 32) && (key <= 125) && (newPassLetterCount < 25)) 
                {
                    newPass[newPassLetterCount] = (char)key;
                    newPass[newPassLetterCount + 1] = '\0';
                    newPassLetterCount++;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) 
            {
                newPassLetterCount--;
                if (newPassLetterCount < 0) newPassLetterCount = 0;
                newPass[newPassLetterCount] = '\0';
            }
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (validator.maleOrFemale(currentUser))
        {
            DrawTexture(profileTemplateMan, 0, 0, RAYWHITE);
        }

        else
        {
            DrawTexture(profileTemplateWoman, 0, 0, RAYWHITE);
        }


        DrawText(currentUser, GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 20, 40, BLACK);
        DrawText(oldPass, oldPassBox.x + 5, oldPassBox.y + 8, 40, BLACK);
        DrawText(TextFormat("%.*s", newPassLetterCount, "**************************"), newPassBox.x + 5, newPassBox.y + 8, 40, BLACK);
        
        DrawRectangle(0, 930, 900, 200, BLACK);

        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(dashboardIcon, dashboardButton.x + 15, dashboardButton.y , RAYWHITE);
        if (isMouseOverDashboardButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            dashboard();
        }

        bool isMouseOverBudgetButton = CheckCollisionPointRec(mousePosition, budgetButton);
        DrawRectangleRounded(budgetButton, 10, int(2), (isMouseOverBudgetButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(budgetIcon, budgetButton.x + 17, budgetButton.y + 10, RAYWHITE);
        if (isMouseOverBudgetButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            budget();
        }

        bool isMouseOverStatisticsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatisticsButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(statisticsIcon, statisticsButton.x + 30, statisticsButton.y + 17, RAYWHITE);
        if (isMouseOverStatisticsButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            statistics();
        }

        bool isMouseOverExitButton = CheckCollisionPointRec(mousePosition, exitButton);
        DrawRectangleRounded(exitButton, 10, int(2), (isMouseOverExitButton ? RED : DARKGRAY));
        DrawText("X", exitButton.x + 18, exitButton.y + 17, 25, BLACK);
        if (isMouseOverExitButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            CloseWindow();
        }

        bool isEditButtonPressed = CheckCollisionPointRec(mousePosition, editButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (strlen(oldPass) > 0 && strlen(newPass) > 0 && isEditButtonPressed)
        {
            if (editPassword(currentUser, oldPass, newPass)) 
            {
                DrawText("Password updated successfully.", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 150, 30, GREEN);
             
                oldPass[0] = '\0';
                newPass[0] = '\0';
                oldPassLetterCount = 0;
                newPassLetterCount = 0;
            }

            else 
            {
                DrawText("Incorrect old password or the new one is not meeting the criteria", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 + 150, 30, RED);
                
                oldPass[0] = '\0';
                newPass[0] = '\0';
                oldPassLetterCount = 0;
                newPassLetterCount = 0;
            }
        }

        EndDrawing();

    }

    UnloadTexture(profileTemplateMan);
    UnloadTexture(profileTemplateWoman);
}