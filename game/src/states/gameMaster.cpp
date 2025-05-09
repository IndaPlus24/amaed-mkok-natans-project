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
#include "score.h"
#include "map.h"

GameData gameData;

void InitGM(dataGM initdata)
{
    // Initiate floor, room, enemies, player, and so on.
    gameData.player = CreatePlayer(Vector2{(float)(tileSize * 25), (float)(tileSize * 15)});


    gameData.player.sheets[0] = LoadSpriteSheet("assets/sprites/n0llan.png", 8, 1);

    gameData.map = CreateMap(3,6,50,30,1,&gameData);
    gameData.currentRoom = &gameData.map.rooms[1];
    gameData.enemies = gameData.map.enemies[1];
    printf("Current room: %d\n", gameData.currentRoom );
    
    gameData.projectiles.count = 0;
    gameData.projectiles.capacity = 16;
    gameData.projectiles.list = (Projectile *) malloc(sizeof(Projectile) * gameData.projectiles.capacity);
    Font arcadeFont = LoadFont("assets/PressStart2P-Regular.ttf");
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

    ScoreDraw();

    for(int i = 0; i < gameData.enemies.count; i++)
    {
        EnemyDraw(&gameData.enemies.enemies[i]);
    }


    for (int i = 0; i <  gameData.projectiles.count; i++) {
        ProjectileDebugDraw(gameData.projectiles.list + i);
    }

    if (gameData.player.state == PlayerState::Dead) {
        if (gameData.player.animationTime > 5.0f) return GameState::GameOver;

        DrawRectangle(0,0, GetScreenWidth(), GetScreenHeight(), Color{0,0,0, (unsigned char)((gameData.player.animationTime / 5) * 255)});
    }

    DrawText("Test Room.", 0, 0, 20, WHITE);
    //draws the framerate
    DrawText(TextFormat("FPS: %i", GetFPS()), 0, 40, 20, WHITE);

    EndDrawing();

    return GameState::GameMaster;
}