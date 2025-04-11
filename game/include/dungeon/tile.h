#ifndef TILE_H
#define TILE_H


#include "raylib.h"

struct Tile
{
    bool isWalkable;
    bool isDoor;
    bool isWall;
};

#endif