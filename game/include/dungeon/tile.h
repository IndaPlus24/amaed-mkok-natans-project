#ifndef TILE_H
#define TILE_H

#include "map.h"

// ---------------------
// Macro Definitions
// ---------------------
#define tileSize 32         // Size of a tile in pixels

struct Tile
{
    bool walkable;  // Set to false for obstacles.
    float cost;     // The "distance" cost from this cell to the player's position.
    Door* door; // Poiner to the door if there is one in this tile. Otherwise, nullptr.
    bool isWall;
};

#endif