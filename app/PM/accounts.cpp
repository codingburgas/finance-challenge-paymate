#include "pch.h"
#include "accounts.h"
#include "accessData.h"
#include "validation.h"
#include "dashboard.h"
void signup()
{
    Texture2D manBigSize = LoadTexture("../images/man.png");
    Texture2D womanBigSize = LoadTexture("../images/woman.png");
    Texture2D background = LoadTexture("../images/signup.png");

    int newWidth = manBigSize.width / 2 - 75;
    int newHeight = manBigSize.height / 2 - 75;

    Image manImage = LoadImage("../images/man.png"); 
    Image womanImage = LoadImage("../images/woman.png"); 
    ImageResize(&manImage, newWidth, newHeight);           
    ImageResize(&womanImage, newWidth, newHeight);           
    Texture2D man = LoadTextureFromImage(manImage); 
    Texture2D woman = LoadTextureFromImage(womanImage); 
    UnloadImage(manImage); 
    UnloadImage(womanImage); 
    char username[25] = "";
    char password[25] = "";
    char repeatPassword[25] = "";
    string gender = "";

    int usernameLetterCount = 0;

    int passwordLetterCount = 0;

    int repeatPasswordLetterCount = 0;

    const Rectangle usernameBox = { GetScreenWidth() / 2 - 350, GetScreenHeight() / 2 - 180 , 550, 65 };

    const Rectangle passwordBox = { GetScreenWidth() / 2- 350, GetScreenHeight() / 2+ 20 , 550, 65 };

    const Rectangle repeatPasswordBox = { GetScreenWidth() / 2- 350, GetScreenHeight() / 2 +180, 550, 65 };

    const Rectangle genderBoxMale = { GetScreenWidth() / 2- 350 , GetScreenHeight() / 2 +250, 100, 65 };
    const Rectangle genderBoxFemale = { GetScreenWidth() / 2- 275, GetScreenHeight() / 2 +245, 100, 65 };

    Rectangle inputBoxes[] = { usernameBox, passwordBox, repeatPasswordBox };

    bool mouseOnInputBox[3] = { false };

    const Rectangle signupButton = { GetScreenWidth() / 2 + 250 , GetScreenHeight() / 2 + 220, 100, 45 };

    DataAccess account;

    SetTargetFPS(60);

    while (!WindowShouldClose())

    {

        Vector2 mousePosition = GetMousePosition();

        // Input box hover check

        for (int i = 0; i < 3; ++i) {

            mouseOnInputBox[i] = CheckCollisionPointRec(mousePosition, inputBoxes[i]);

        }

        bool anyInputBoxHovered = false;

        for (int i = 0; i < 3; ++i) {

            if (mouseOnInputBox[i]) {

                anyInputBoxHovered = true;

                break;

            }

        }

        SetMouseCursor(anyInputBoxHovered ? MOUSE_CURSOR_IBEAM : MOUSE_CURSOR_DEFAULT);

        // Input handling

        for (int i = 0; i < 3; ++i) {

            if (mouseOnInputBox[i]) {

                int key = GetCharPressed();

                while (key > 0) {

                    if ((key >= 32) && (key <= 125)) {

                        switch (i) {

                        case 0: // username box

                            if (usernameLetterCount < 50) {

                                username[usernameLetterCount++] = (char)key;

                                username[usernameLetterCount] = '\0';

                            }

                            break;

                        case 1: // password box

                            if (passwordLetterCount < 50) {

                                password[passwordLetterCount++] = (char)key;

                                password[passwordLetterCount] = '\0';

                            }

                            break;

                        case 2: // repeat password box

                            if (repeatPasswordLetterCount < 50) {

                                repeatPassword[repeatPasswordLetterCount++] = (char)key;

                                repeatPassword[repeatPasswordLetterCount] = '\0';

                            }

                            break;

                        }

                    }

                    key = GetCharPressed();

                }

                if (IsKeyPressed(KEY_BACKSPACE))

                {

                    switch (i) {

                    case 0: // username box

                        if (usernameLetterCount > 0) {

                            usernameLetterCount--;

                            username[usernameLetterCount] = '\0';

                        }

                        break;

                    case 1: // password box

                        if (passwordLetterCount > 0) {

                            passwordLetterCount--;

                            password[passwordLetterCount] = '\0';

                        }

                        break;

                    case 2: // repeat password box

                        if (repeatPasswordLetterCount > 0) {

                            repeatPasswordLetterCount--;

                            repeatPassword[repeatPasswordLetterCount] = '\0';

                        }

                        break;

                    }

                }

            }

        }

        Validate validator;
        if (CheckCollisionPointRec(mousePosition, genderBoxMale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            gender = "male";
        }
        else if (CheckCollisionPointRec(mousePosition, genderBoxFemale) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            gender = "female";
        }
        bool showMismatchError = false; // Flag for mismatched passwords

        bool showRequirementError = false; // Flag for requirement errors

        // Sign up button click check

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, signupButton)) {

            if (strcmp(password, repeatPassword) != 0) {

                // Clear password fields if passwords don't match

                password[0] = '\0';

                repeatPassword[0] = '\0';

                passwordLetterCount = 0;

                repeatPasswordLetterCount = 0;

                showMismatchError = true; // Set mismatch error flag

                showRequirementError = false; // Ensure requirement error is not shown

            }

            else {

                // Check password requirements if passwords match

                if (validator.containsUppercase(password) && validator.containsLowercase(password) &&

                    validator.containsDigit(password) && validator.containsSpecial(password)) {

                    account.addAccount(username,password,gender);

                    strcpy_s(currentUser, username);

                    dashboard();

                }

                else {

                    // Password does not meet requirements, handle accordingly

                    password[0] = '\0';

                    repeatPassword[0] = '\0';

                    passwordLetterCount = 0;

                    repeatPasswordLetterCount = 0;

                }

            }

        }


        // Drawing

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(background, 0, 0, RAYWHITE);

        DrawText(username, usernameBox.x + 5, usernameBox.y + 8, 40, BLACK);

        DrawText(TextFormat("%.*s", passwordLetterCount, "******************************************"), passwordBox.x + 5, passwordBox.y + 8, 40, BLACK);

        DrawText(TextFormat("%.*s", repeatPasswordLetterCount, "******************************************"), repeatPasswordBox.x + 5, repeatPasswordBox.y + 8, 40, BLACK);
        DrawTexture(man, genderBoxMale.x, genderBoxMale.y, RAYWHITE);
        DrawTexture(woman, genderBoxFemale.x, genderBoxFemale.y, RAYWHITE);
        bool isMouseOverButtonLogin = CheckCollisionPointRec(mousePosition, signupButton);
        DrawText("Enter", GetScreenWidth() / 2 + 275, GetScreenHeight() / 2 + 260, 30, isMouseOverButtonLogin ? RED : WHITE);

        EndDrawing();

    }

}

void login()

{

    Texture2D background = LoadTexture("../images/login.png");

    bool isAnswerTrue = false;

    char username[25] = "\0";

    char password[25] = "\0";

    int usernameLetterCount = 0;

    int passwordLetterCount = 0;

    const Rectangle usernameBox = { GetScreenWidth() / 2 - 350, GetScreenHeight() / 2 - 165 , 550, 65 };

    const Rectangle passwordBox = { GetScreenWidth() / 2 - 350, GetScreenHeight() / 2 + 40 , 550, 65 };

    bool mouseOnUsername = false;

    bool mouseOnPassword = false;

    const Rectangle loginButton = { GetScreenWidth() / 2 + 250, GetScreenHeight() / 2 + 70, 100, 45 };



    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        Vector2 mousePosition = GetMousePosition();

        if (CheckCollisionPointRec(mousePosition, usernameBox)) mouseOnUsername = true;

        else mouseOnUsername = false;

        if (CheckCollisionPointRec(mousePosition, passwordBox)) mouseOnPassword = true;

        else mouseOnPassword = false;

        if (mouseOnUsername || mouseOnPassword) SetMouseCursor(MOUSE_CURSOR_IBEAM);

        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnUsername) {

            int key = GetCharPressed();

            while (key > 0) {

                if ((key >= 32) && (key <= 125) && (usernameLetterCount < 25)) {

                    username[usernameLetterCount] = (char)key;

                    username[usernameLetterCount + 1] = '\0';

                    usernameLetterCount++;

                }

                key = GetCharPressed();

            }

            if (IsKeyPressed(KEY_BACKSPACE)) {

                usernameLetterCount--;

                if (usernameLetterCount < 0) usernameLetterCount = 0;

                username[usernameLetterCount] = '\0';

            }

        }

        if (mouseOnPassword) {

            int key = GetCharPressed();

            while (key > 0) {

                if ((key >= 32) && (key <= 125) && (passwordLetterCount < 25)) {

                    password[passwordLetterCount] = (char)key;

                    password[passwordLetterCount + 1] = '\0';

                    passwordLetterCount++;

                }

                key = GetCharPressed();

            }

            if (IsKeyPressed(KEY_BACKSPACE)) {

                passwordLetterCount--;

                if (passwordLetterCount < 0) passwordLetterCount = 0;

                password[passwordLetterCount] = '\0';

            }

        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePosition, loginButton) || IsKeyPressed(KEY_ENTER)) {

            Validate validator;
           // if (validator.doesAccountExist(username))
            //{
                if (validator.isPasswordCorrect(username, password))
                {
                    strcpy_s(currentUser, username);
                    dashboard();
                }
            //}

            else

            {

                password[0] = '\0';

                username[0] = '\0';

                passwordLetterCount = 0;

                usernameLetterCount = 0;

            }

        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(background, 0, 0, WHITE);
        DrawText(username, usernameBox.x + 5, usernameBox.y + 8, 40, BLACK);
        DrawText(TextFormat("%.*s", passwordLetterCount, "**************************"), passwordBox.x + 5, passwordBox.y + 8, 40, BLACK);
        bool isMouseOverButtonLogin = CheckCollisionPointRec(mousePosition, loginButton);
        DrawText("Enter", GetScreenWidth() / 2 + 260, GetScreenHeight() / 2 + 100, 30, isMouseOverButtonLogin ? RED : WHITE);
        EndDrawing();

    }

}


void startingScreen()

{

    const int screenWidth = 900;

    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "PM");

    Vector2 loginButtonPosition = { GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 450 };

    Vector2 signUpButtonPosition = { GetScreenWidth() / 2 - 225, GetScreenHeight() / 2 - 475 };

    Vector2 exitButtonPosition = { GetScreenWidth() / 2 + 175, GetScreenHeight() / 2 + 425 };

    const Rectangle loginButton = { loginButtonPosition.x, loginButtonPosition.y, 125, 100 };

    const Rectangle signUpButton = { signUpButtonPosition.x, signUpButtonPosition.y, 200, 100 };

    const Rectangle exitButton = { exitButtonPosition.x, exitButtonPosition.y, 200, 100 };

    Texture2D background = LoadTexture("../images/homepage.png");

    SetTargetFPS(60);

    while (!WindowShouldClose())

    {

        Vector2 mousePosition = GetMousePosition();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawTexture(background, 0, 0, RAYWHITE);

        bool isMouseOverLoginButton = CheckCollisionPointRec(mousePosition, loginButton);

        DrawText("Login", loginButtonPosition.x, loginButtonPosition.y, 50, isMouseOverLoginButton ? RED : WHITE);

        if (CheckCollisionPointRec(mousePosition, loginButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            login();

        }


        bool isMouseOverSignUpButton = CheckCollisionPointRec(mousePosition, signUpButton);

        DrawText("Sign up", signUpButtonPosition.x + 60, signUpButtonPosition.y + 25, 50, isMouseOverSignUpButton ? RED : WHITE);

        if (CheckCollisionPointRec(mousePosition, signUpButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            signup();

        }

        bool isMouseOverExitButton = CheckCollisionPointRec(mousePosition, exitButton);

        DrawText("Exit", exitButtonPosition.x + 100, exitButtonPosition.y + 25, 50, isMouseOverExitButton ? RED : WHITE);

        if (CheckCollisionPointRec(mousePosition, exitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

            return;

        }

        EndDrawing();

    }

}
