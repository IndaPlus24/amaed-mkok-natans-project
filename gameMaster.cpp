#include <raylib.h>

#include "gameMaster.h"
#include "input.h"

void InitGM(dataGM initdata)
{
    // Initiate floor, room, enemies, player, and so on.
}

GameState RunGM()
{
    // Get inputs
    Inputs inputs = GetInputs();

    // PlayerAct(&player, inputs, &room, &listOfEnemies);

    // AllEnemiesAct(ArrayOfEnemies, &room, &player);

    // nextRoom = FunctionThatReturnsWhatRoomThePlayerIsGoingToOrNull();

    // if (nextRoom != NULL) {
    //     LoadRoom(nextRoom);
    // }

    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("This has not been implemented yet, please quit.", 0, 0, 60, WHITE);
    DrawText("(Press Escape)", 0, 60, 60, WHITE);

    // Draw everything!

    EndDrawing();

    return GameState::GameMaster;
}