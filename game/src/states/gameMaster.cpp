#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "gameMaster.h"
#include "input.h"
#include "gameData.h"
#include "playerFunks.h"
#include "enemiesFunks.h"


GameData gameData;
Room a;

void InitGM(dataGM initdata)
{
    // Initiate floor, room, enemies, player, and so on.
    gameData.player = CreatePlayer(Vector2{(float)(tileSize * 25), (float)(tileSize * 15)});
    Vector2 enemyPos[2] = {Vector2{(float)(tileSize * 10), (float)(tileSize * 10)}, Vector2{(float)(tileSize * 15), (float)(tileSize * 15)}};
    EnemyType enemyTypes[2] = {ENEMY_MELEE, ENEMY_MELEE};
    EnemyBehavior enemyBehaviors[2] = {BEHAVIOR_RUSH, BEHAVIOR_RUSH};

    gameData.enemies = CreateEnemies(CreateEnemySeeder(2, enemyPos, enemyTypes , enemyBehaviors));

    gameData.player.sheets[0] = LoadSpriteSheet("assets/sprites/n0llan.png", 8, 1);

    //a = CreateRoom(0, 20, 20);
    a = DrunkardsWalk(0, 50, 30, 150);
    
    gameData.currentRoom = &a;

    BeginDrawing();
    EndDrawing();
}

GameState RunGM()
{
    // Get inputs
    Inputs inputs = GetInputs();

    PlayerUpdate(&gameData, &inputs);
    //UPDATES ALL ENEMIES   
    for (int i = 0; i < gameData.enemies.count; i++)
    {
        EnemyUpdate(&gameData.enemies.enemies[i], &gameData);
    }
    EnemyUpdate(&gameData.enemies.enemies[0], &gameData);

    // AllEnemiesAct(ArrayOfEnemies, &room, &player);

    // nextRoom = FunctionThatReturnsWhatRoomThePlayerIsGoingToOrNull();

    // if (nextRoom != NULL) {
    //     LoadRoom(nextRoom);
    // }

    BeginDrawing();
    ClearBackground(BLACK);

    RoomDraw(gameData.currentRoom);
    PlayerDraw(&gameData.player);
    EnemyDraw(&gameData.enemies.enemies[0]);
    EnemyDraw(&gameData.enemies.enemies[1]);

    DrawText("Test Room", 0, 0, 20, WHITE);

    // Draw everything!

    EndDrawing();

    return GameState::GameMaster;
}