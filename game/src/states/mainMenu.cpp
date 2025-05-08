#include <stdbool.h>

#include <string.h>
#include <raylib.h>

#include "mainMenu.h"
#include "gameMaster.h"
#include "input.h"

int selection;
unsigned menuId;
GameState changeState;

/// @brief Initiate and reset the the Main Menu
void InitMM()
{
    selection = 0;
    menuId = 0;
    changeState = GameState::MainMenu;
}

/// @brief Change menu category
/// @param menu
void GotoMenu(int menu)
{
    selection = 0;
    menuId = menu;
}

bool TryPickOption(Inputs &inputs)
{
    return inputs.a == ButtonState::Pressed;
}

bool TryGoBack(Inputs &inputs)
{
    return inputs.b == ButtonState::Pressed;
}

void navigate(int options, Inputs &inputs)
{
    if (inputs.down == ButtonState::Pressed)
    {
        selection = (selection + 1) % options;
    }
    if (inputs.up == ButtonState::Pressed)
    {
        selection = (selection - 1 + options) % options;
    }
}

/// @brief Did you know?: the dialogue in UNDERTALE is handled with one gigantic switch statement.
void navigateFuckedUpAndEvilMenuTree()
{
    Inputs inputs = GetInputs();
    switch (menuId)
    {
    // Start menu: Start, Options, Exit
    case 0:
        if (TryPickOption(inputs))
        {
            switch (selection)
            {
            case 0:
                changeState = GameState::GameMaster;
                break;
            case 1:
                GotoMenu(1);
                break;
            case 2:
                // Exit
                changeState = GameState::Break;
                break;
            case 3:
                changeState = GameState::HighScore;
                break;

            }
        }
        else
        {
            (navigate(4, inputs));
        }
        break;
    // Options: change volume, back
    case 1:
        if (selection == 0 && (inputs.right == ButtonState::Pressed || inputs.left == ButtonState::Pressed))
        {
            int change = (inputs.right == ButtonState::Pressed) - (inputs.left == ButtonState::Pressed);
            SetMasterVolume(GetMasterVolume() + change);
        }
        else if ((selection == 1 && TryPickOption(inputs)) || TryGoBack(inputs))
        {
            GotoMenu(0);
        }
        else
        {
            navigate(2, inputs);
        }
        break;
    }
}

/// @brief Draw The given options, highlighting the selected one.
/// @param options List of option labels.
/// @param amount The number of options.
void drawOptions(const char *options[], int amount)
{
    int textSize = 50;
    int verticalPadding = 5;
    int halfW = GetScreenWidth() >> 1;
    int yOffset = (GetScreenHeight() >> 1) - ((amount * (textSize + verticalPadding * 2)) >> 2);

    int selectionPadding = 5;

    for (int i = 0; i < amount; i++)
    {
        int textWidth = MeasureText(options[i], textSize);
        int x = halfW - (textWidth >> 1);
        int y = yOffset + (textSize + verticalPadding * 2) * i;

        if (i == selection)
        {
            DrawRectangle(x - selectionPadding, y - verticalPadding, textWidth + selectionPadding * 2, textSize + verticalPadding * 2, WHITE);
            DrawText(options[i], x, y, textSize, BLACK);
        }
        else
        {
            DrawText(options[i], x, y, textSize, WHITE);
        }
    }
}

void drawFuckedUpAndEvilMenuTree()
{
    const char *options[10];
    int amount = 0;

    switch (menuId)
    {
    case 0:
        options[0] = "Start";
        options[1] = "Inställningar";
        options[2] = "Avsluta";
        options[3] = "High Scores";        
        amount = 4;
        break;

    case 1:
        options[0] = TextFormat("- volym: %.f +", GetMasterVolume());
        options[1] = "Tillbaka";
        amount = 2;
        break;
    }
    drawOptions(options, amount);
}

GameState runMM()
{
    navigateFuckedUpAndEvilMenuTree();

    switch (changeState)
    {
    case GameState::HighScore:
        return GameState::HighScore;
    case GameState::Break:
        return GameState::Break;
    case GameState::GameMaster:
        dataGM d;
        InitGM(d);
        InitMM();
        BeginDrawing();
        EndDrawing();
        return GameState::GameMaster;
    default:
        break;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    drawFuckedUpAndEvilMenuTree();

    EndDrawing();
    return GameState::MainMenu;
}