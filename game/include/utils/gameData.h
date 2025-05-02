#ifndef GAME_DATA_H
#define GAME_DATA_H

#include "playerStruct.h"
#include "enemies.h"
#include "map.h"

typedef struct GameData {
    Player player;
    Enemies enemies;
    Room currentRoom;
    Map map;
} GameData;

#endif