#include <raylib.h>

#include "gameMaster.h"
#include "input.h"
#include "gameData.h"
#include "playerFunks.h"
#include "projectilesFunks.h"

GameData gameData;
Room a;

void InitGM(dataGM initdata)
{
    // Initiate floor, room, enemies, player, and so on.
    gameData.player = CreatePlayer(Vector2{(float)(tileSize * 4), (float)(tileSize * 4)});

    gameData.player.sheets[0] = LoadSpriteSheet("assets/sprites/n0llan.png", 8, 1);

    a = CreateRoom(0, 20, 20);
    gameData.currentRoom = &a;

    gameData.enemies.count = 0;
    gameData.enemies.enemies = (Enemy *) malloc(sizeof(Enemy) * 100);
    
    gameData.projectiles.count = 0;
    gameData.projectiles.capacity = 16;
    gameData.projectiles.list = (Projectile *) malloc(sizeof(Projectile) * gameData.projectiles.capacity);

    
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

    for (int i = - gameData.projectiles.count; i < 0; i++) {
        ProjectileUpdate(gameData.projectiles.list + gameData.projectiles.count + i, &gameData);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    RoomDraw(gameData.currentRoom);
    PlayerDraw(&gameData.player);

    for (int i = 0; i <  gameData.projectiles.count; i++) {
        ProjectileDebugDraw(gameData.projectiles.list + i);
    }

    DrawText("Test Room", 0, 0, 20, WHITE);

    // Draw everything!

    EndDrawing();

    return GameState::GameMaster;
}