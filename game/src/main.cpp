#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#include "mainMenu.h"
#include "gameMaster.h"
#include "score.h"

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
        case GameState::HighScore:
        {
            std::vector<ScoreEntry> scores = GetHighScores();
            BeginDrawing();
            ClearBackground(BLACK);
            char text[] = "HIGH SCORE";
            int fontSize = 60;
            int verticalPadding = 5;
            int horizontalPadding = 10;
            int width = MeasureText(text, fontSize);
            DrawText(text, (GetScreenWidth() - width) / 2, (fontSize + 10),fontSize, RED);

            int startY = 200;
            int scoreWidth =  MeasureText("AAA 00000", fontSize);
            for (size_t i = 0; i < scores.size(); ++i) {
                std::string text = scores[i].initials + " " + std::to_string(scores[i].score);
                DrawText(text.c_str(), (GetScreenWidth() - scoreWidth) / 2, startY + i * (fontSize + verticalPadding), fontSize, WHITE);
            }

            //int widthBack = MeasureText("Tillbaka", fontSize);
            //DrawRectangle((GetScreenWidth() - widthBack)/ 2-horizontalPadding, (GetScreenHeight() - 30 - fontSize) - verticalPadding, widthBack + horizontalPadding * 2, fontSize + verticalPadding * 2, WHITE);
            //DrawText("Tillbaka", (GetScreenWidth() - widthBack) / 2, ( GetScreenHeight() - 30 - fontSize), fontSize, BLACK);


            EndDrawing();
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
            int scoreWidth = MeasureText("Poäng: 00000", fontSize);
            DrawText(GetFormatScore(), (GetScreenWidth() - scoreWidth) / 2, (GetScreenHeight() - fontSize) / 2 - fontSize, fontSize, WHITE);
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