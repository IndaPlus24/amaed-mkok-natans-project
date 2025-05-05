#ifndef TILE_H
#define TILE_H

// ---------------------
// Macro Definitions
// ---------------------
#define tileSize 32         // Size of a tile in pixels

struct Tile
{
    bool walkable;  // Set to false for obstacles.
    float cost;     // The "distance" cost from this cell to the player's position.
    bool isDoor;
    bool isWall;
};

#endif