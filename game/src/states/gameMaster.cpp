#include <raylib.h>

#include "gameMaster.h"
#include "input.h"
#include "gameData.h"
#include "playerFunks.h"

GameData gameData;
Room a;

void InitGM(dataGM initdata)
{
    // Initiate floor, room, enemies, player, and so on.
    gameData.player = CreatePlayer(Vector2{(float)(tileSize * 4), (float)(tileSize * 4)});

    gameData.player.sheets[0] = LoadSpriteSheet("assets/sprites/n0llan.png", 8, 1);

    a = CreateRoom(0, 20, 20);
    gameData.currentRoom = &a;

    BeginDrawing();
    EndDrawing();
}

GameState RunGM()
{
    // Get inputs
    Inputs inputs = GetInputs();

    PlayerUpdate(&gameData, &inputs);

    // AllEnemiesAct(ArrayOfEnemies, &room, &player);

    // nextRoom = FunctionThatReturnsWhatRoomThePlayerIsGoingToOrNull();

    // if (nextRoom != NULL) {
    //     LoadRoom(nextRoom);
    // }

    BeginDrawing();
    ClearBackground(BLACK);

    RoomDraw(gameData.currentRoom);
    PlayerDraw(&gameData.player);

    DrawText("Test Room", 0, 0, 20, WHITE);

    // Draw everything!

    EndDrawing();

    return GameState::GameMaster;
}