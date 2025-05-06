#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#include "mainMenu.h"
#include "gameMaster.h"

void InitDisplay()
{
    SetTraceLogLevel(LOG_NONE);
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    InitWindow(w, h, "AQoQ"); // A-Quart och Quartakomber (temp name, reference to AD&D)
    ToggleFullscreen();
}

int main()
{
    GameState gameState = GameState::MainMenu;
    time_t t;
    srand((unsigned)time(&t));

    InitDisplay();

    // Load textures or something

    while (!WindowShouldClose())
    {
        switch (gameState)
        {
        case GameState::MainMenu:
        {
            gameState = runMM();
        }
        break;
        case GameState::GameMaster:
        {
            gameState = RunGM();
        }
        break;
        case GameState::GameOver:
        {
            // Remove this and replace it with the "real shit(tm)"
            BeginDrawing();
            ClearBackground(BLACK);
            char text[] = "GAME OVER";
            int fontSize = 50;
            int width = MeasureText(text, fontSize);
            DrawText(text, (GetScreenWidth() - width) / 2, (GetScreenHeight() - fontSize) / 2, fontSize, WHITE);
            EndDrawing();
        }
        break;
        case GameState::Break:
            CloseWindow();
            return 0;
        }
    }
    CloseWindow();
    return 0;
}