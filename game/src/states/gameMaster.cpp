#include <raylib.h>

#include "gameMaster.h"
#include "input.h"
#include "gameData.h"
#include "playerFunks.h"
#include "projectilesFunks.h"
#include "enemiesFunks.h"

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

    if (gameData.player.state == PlayerState::Dead) {
        if (gameData.player.animationTime > 5.0f) return GameState::GameOver;

        DrawRectangle(0,0, GetScreenWidth(), GetScreenHeight(), Color{0,0,0, (unsigned char)((gameData.player.animationTime / 5) * 255)});
    }

    DrawText("Test Room.", 0, 0, 20, WHITE);
    

    EndDrawing();

    return GameState::GameMaster;
}