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
   
    Validate validator;

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
        
        bool isMouseOverButtonLogin = CheckCollisionPointRec(mousePosition, editButton);
        DrawText("Save", GetScreenWidth() / 2 + 260, GetScreenHeight() / 2 + 300, 30, isMouseOverButtonLogin ? BLACK : WHITE);
        DrawRectangle(0, 930, 900, 200, BLACK);

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

        bool isMouseOverStatisticsButton = CheckCollisionPointRec(mousePosition, statisticsButton);
        DrawRectangleRounded(statisticsButton, 10, int(2), (isMouseOverStatisticsButton ? DARKGRAY : LIGHTGRAY));
        if (CheckCollisionPointRec(mousePosition, statisticsButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            statistics();
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