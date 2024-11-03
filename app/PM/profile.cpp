#include "profile.h"
#include "dashboard.h"
#include "budget.h"
#include "statistics.h"
#include "validation.h"
#include "accessData.h"

// Function to edit a user's password
bool editPassword(const string& username, const string& oldPass, const string& newPass) 
{
    ifstream file("../data/accounts.csv");
    vector<string> lines;
    string line, storedUsername, storedPassword, storedGender;
    bool passwordUpdated = false;
    Validate validator;

    if (!file.is_open())
    {
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
    Font font = LoadFont("../font/font.ttf");

    const int screenWidth = 900;
    const int screenHeight = 1080;

    const Rectangle dashboardButton = { 150, 970, 140, 75 };
    const Rectangle budgetButton = { 380, 970, 140, 75 };
    const Rectangle statisticsButton = { 610, 970, 140, 75 };
   
    // Load profile templates based on gender
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

        // Check mouse position for input boxes
        if (CheckCollisionPointRec(mousePosition, oldPassBox)) mouseOnOldPass = true;
        else mouseOnOldPass = false;

        if (CheckCollisionPointRec(mousePosition, newPassBox)) mouseOnNewPass = true;
        else mouseOnNewPass = false;

        if (mouseOnOldPass || mouseOnNewPass) SetMouseCursor(MOUSE_CURSOR_IBEAM);
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        // Handle old password input
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

        // Handle new password input
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

        // Display the appropriate profile template
        if (validator.maleOrFemale(currentUser))
        {
            DrawTexture(profileTemplateMan, 0, 0, RAYWHITE);
        }

        else
        {
            DrawTexture(profileTemplateWoman, 0, 0, RAYWHITE);
        }

        // Display current user name and password inputs
        DrawTextEx(font, currentUser, Vector2{ (float)GetScreenWidth() / 2 - 90, (float)GetScreenHeight() / 2 - 20 }, 40, 2, BLACK);
        DrawTextEx(font, oldPass, Vector2{ (float)oldPassBox.x + 5, (float)oldPassBox.y + 8 }, 40, 2, BLACK);
        DrawTextEx(font, TextFormat("%.*s", newPassLetterCount, "**************************"), Vector2{ (float)newPassBox.x + 5, (float)newPassBox.y + 8 }, 40, 2, BLACK);
       
        bool isMouseOverButtonLogin = CheckCollisionPointRec(mousePosition, editButton);
        DrawTextEx(font, "Save", Vector2{ (float)GetScreenWidth() / 2 + 260, (float)GetScreenHeight() / 2 + 300 }, 30, 2, isMouseOverButtonLogin ? BLACK : WHITE);
        
        DrawRectangle(0, 930, 900, 200, BLACK);

        // Draw navigation buttons
        bool isMouseOverDashboardButton = CheckCollisionPointRec(mousePosition, dashboardButton);
        DrawRectangleRounded(dashboardButton, 10, int(2), (isMouseOverDashboardButton ? DARKGRAY : LIGHTGRAY));
        DrawTexture(dashboardIcon, dashboardButton.x + 15, dashboardButton.y , RAYWHITE);
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
        DrawTexture(statisticsIcon, statisticsButton.x + 30, statisticsButton.y + 17, RAYWHITE);
        if (isMouseOverStatisticsButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            statistics();
        }

        bool isMouseOverExitButton = CheckCollisionPointRec(mousePosition, exitButton);
        DrawRectangleRounded(exitButton, 10, int(2), (isMouseOverExitButton ? RED : DARKGRAY));
        DrawTextEx(font, "X", Vector2{ (float)exitButton.x + 18, (float)exitButton.y + 15 }, 25, 2, BLACK);
        if (isMouseOverExitButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            CloseWindow();
        }

        // Check if edit button is pressed to update password
        bool isEditButtonPressed = CheckCollisionPointRec(mousePosition, editButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        if (strlen(oldPass) > 0 && strlen(newPass) > 0 && isEditButtonPressed)
        {
            if (editPassword(currentUser, oldPass, newPass)) 
            {
                DrawTextEx(font, "Password updated successfully.", Vector2{ (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 150 }, 30, 2, GREEN);
             
                // Clear input boxes after successful update
                oldPass[0] = '\0';
                newPass[0] = '\0';
                oldPassLetterCount = 0;
                newPassLetterCount = 0;
            }

            else 
            {
                DrawTextEx(font, "Incorrect old password or the new one is not meeting the criteria", Vector2{ (float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 150 }, 30, 2, RED);
                
                // Clear input boxes after failed update
                oldPass[0] = '\0';
                newPass[0] = '\0';
                oldPassLetterCount = 0;
                newPassLetterCount = 0;
            }
        }
        EndDrawing();
    }

    //Unload all textures
    UnloadTexture(profileTemplateMan);
    UnloadTexture(profileTemplateWoman);
    UnloadTexture(dashboardPhoto);
    UnloadTexture(statisticsPhoto);
    UnloadTexture(budgetPhoto);
    UnloadTexture(dashboardIcon);
    UnloadTexture(statisticsIcon);
    UnloadTexture(budgetIcon);
    //Unload font
    UnloadFont(font);
}