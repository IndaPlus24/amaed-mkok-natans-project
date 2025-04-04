#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#include "input.cpp"

typedef enum GameState
{
    STATE_MENU,
    STATE_GAME,
    STATE_GAMEOVER
} GameState;

bool rectCollision(Rectangle rect, Vector2 point)
{
    return (rect.x < point.x && rect.x + rect.width > point.x && rect.y < point.y && rect.y + rect.height > point.y);
}

int main()
{
    GameState gameState = STATE_MENU;
    time_t t;
    srand((unsigned)time(&t));

    // ----------------
    // DISPLAY SETTINGS
    // ----------------
    InitWindow(1000, 1000, "DJam");
    // ToggleFullscreen();
    // SetTargetFPS(10);

    // Load textures
    Texture2D worldSprites = LoadTexture("Assets/world.png");

    // Initialize inputs
    Inputs inputs = {0};
    inputs = GetInputs();

    // Initialize room
    Room room;
    RoomGrid roomGrid = RoomCreator(); // Create a room grid, so a 2d array of rooms we want to use or other data structures
    room = roomGrid.data[0][0];

    // ---------
    // GAME LOOP
    // ---------
    while (!WindowShouldClose())
    {
        switch (gameState)
        {
        case STATE_MENU:
        {
            executePressed = IsKeyPressed(KEY_DOWN);
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText(TextFormat("This is where your joureny starts traveller.\nBeware, danger lies this way \n \n Press down arrrow to continue"), 20, roomGridSize * tileSize / 2, 40, (Color){200, 200, 250, 255});
            EndDrawing();
            if (executePressed)
            {
                gameState = STATE_GAME;
            }
        }
        break;
        case STATE_GAME:
        {
            timeElapsed += GetFrameTime();
            room = roomGrid.data[roomPOS.x][roomPOS.y];

            // ------
            // RENDER
            // ------
            BeginDrawing();
            ClearBackground(BLACK);

            // Draw Rooms
            for (int x = 0; x < roomSize; x++)
            {
                for (int y = 0; y < roomSize; y++)
                {
                    switch (room.data[x][y][0])
                    {
                    case TILE_TYPE_WALL:
                        DrawTextureRec(worldSprites, (Rectangle){0, 40, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case TILE_TYPE_DOOR_NORTH:
                        DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, (Color){255, 255, 0, 255});
                        break;
                    case TILE_TYPE_DOOR_EAST:
                        DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, (Color){255, 0, 255, 255});
                        break;
                    case TILE_TYPE_DOOR_SOUTH:
                        DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, (Color){0, 255, 255, 255});
                        break;
                    case TILE_TYPE_DOOR_WEST:
                        DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, (Color){0, 255, 0, 255});
                        break;
                    case TILE_TYPE_EMPTY:
                        DrawTextureRec(worldSprites, (Rectangle){0, 0, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case SCHEDULED_FOR_DELETE:
                        DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, (Color){80, 40, 45, 255});
                    default:
                        break;
                    }

                    switch (room.data[x][y][1])
                    {
                    case TILE_TYPE_WALL_NORTH:
                        DrawTextureRec(worldSprites, (Rectangle){80, 40, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case TILE_TYPE_WALL_EAST:
                        DrawTextureRec(worldSprites, (Rectangle){40, 40, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case TILE_TYPE_WALL_SOUTH:
                        DrawTextureRec(worldSprites, (Rectangle){40, 0, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case TILE_TYPE_WALL_WEST:
                        DrawTextureRec(worldSprites, (Rectangle){80, 0, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case TILE_TYPE_CORNER_NORTH_EAST:
                        DrawTextureRec(worldSprites, (Rectangle){120, 40, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case TILE_TYPE_CORNER_NORTH_WEST:
                        DrawTextureRec(worldSprites, (Rectangle){160, 40, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case TILE_TYPE_CORNER_SOUTH_EAST:
                        DrawTextureRec(worldSprites, (Rectangle){120, 0, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                        break;
                    case TILE_TYPE_CORNER_SOUTH_WEST:
                        DrawTextureRec(worldSprites, (Rectangle){160, 0, 40, 40}, (Vector2){x * tileSize, y * tileSize}, (Color){255, 255, 255, 255});
                    default:
                        break;
                    }
                }
            }

            DrawCircle(playerPosition.x + 1, playerPosition.y + 1, playerRadius, (Color){0, 0, 0, 255});
            DrawCircle(playerPosition.x, playerPosition.y, playerRadius, (Color){100, 255, 100, 255});
            DrawCircle(playerPosition.x - 12, playerPosition.y - 1, 2, (Color){0, 0, 0, 255});
            DrawCircle(playerPosition.x + 12, playerPosition.y - 1, 2, (Color){0, 0, 0, 255});
            DrawRectangle(playerPosition.x - 8, playerPosition.y + 2, 16, 2, (Color){0, 0, 0, 255});

            // Draw Enemies
            for (int i = 0; i < maxEnemies; i++)
            {
                if (enemies[roomPOS.x][roomPOS.y][i].health > 0)
                {
                    if (enemies[roomPOS.x][roomPOS.y][i].type == AITYPE_BOSS)
                    {
                        DrawBoss(enemies[roomPOS.x][roomPOS.y][i].health);
                    }
                    else
                    {
                        DrawEnemy(&enemies[roomPOS.x][roomPOS.y][i]);
                    }
                }
            }

            EndDrawing();
        }
        break;
        case STATE_GAMEOVER:
        {
            BeginDrawing();
            ClearBackground(BLACK);
            DrawText(TextFormat("This is where your journey ends traveller\n your final score was %d, the high score is %d", CalculateScore(roomsCleared, playerHealth, playerMaxHealth, timeElapsed, bossCleared), highscore), 20, roomGridSize * tileSize / 2, 40, (Color){200, 200, 250, 255});
            EndDrawing();
        }
        break;

        default:
            break;
        }
    }
    CloseWindow();
    return 0;
}