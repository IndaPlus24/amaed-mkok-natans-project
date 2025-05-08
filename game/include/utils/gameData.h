#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "playerStruct.h"
#include "enemiesStruct.h"
#include "projectilesStruct.h"
#include "map.h"

struct Map; // Forward declaration of Map struct

typedef struct GameData {
    Player player;
    Enemies enemies;
    Projectiles projectiles;
    Room *currentRoom;
    Map* map;
} GameData;

#endif