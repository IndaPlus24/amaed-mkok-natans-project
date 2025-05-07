#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "gameMaster.h"
#include "input.h"
#include "gameData.h"
#include "playerFunks.h"
#include "projectilesFunks.h"
#include "enemiesFunks.h"

GameData gameData;
// Temporary variable, get rid off it when a proper map has been implemented. - N
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

    a = DrunkardsWalk(0, 50, 30, 150);
    
    gameData.currentRoom = &a;
    
    gameData.projectiles.count = 0;
    gameData.projectiles.capacity = 16;
    gameData.projectiles.list = (Projectile *) malloc(sizeof(Projectile) * gameData.projectiles.capacity);
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

    for (int i = - gameData.projectiles.count; i < 0; i++) {
        ProjectileUpdate(gameData.projectiles.list + gameData.projectiles.count + i, &gameData);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    RoomDraw(gameData.currentRoom);
    
    PlayerDraw(&gameData.player);
    EnemyDraw(&gameData.enemies.enemies[0]);
    EnemyDraw(&gameData.enemies.enemies[1]);


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