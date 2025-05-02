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
            // idk - Natan
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