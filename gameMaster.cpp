#include <raylib.h>

#include "include/gameMaster.h"
#include "include/input.h"

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

    DrawText("Detta har inte implementerats än, var snäll och stick.", 0, 0, 60, WHITE);
    DrawText("(Tryck på Escape-knappen)", 0, 60, 60, WHITE);

    // Draw everything!

    EndDrawing();

    return GameState::GameMaster;
}