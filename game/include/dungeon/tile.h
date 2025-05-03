#ifndef TILE_H
#define TILE_H

// ---------------------
// Macro Definitions
// ---------------------
#define tileSize 32         // Size of a tile in pixels

struct Tile
{
    bool isWalkable;
    bool isDoor;
    bool isWall;
};

#endif