#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

// TEMPORARY FUNCTION
Room CreateRoom(int id, int width, int height, RoomType type)
{
    Room room;
    room.id = id;
    room.height = height;
    room.width = width;
    room.type = type;
    // room.tags;
    // room.tiles;
    return room;
}

// TEMPORARY FUNCTION
Door CreateDoor(int fromId, int toId)
{
    Door door;
    door.fromRoomId = fromId;
    door.toRoomId = toId;
    // door.fromRoomOffset;
    // door.fromDir;
    // door.toRoomOffset;
    return door;
};

// TEMPORARY FUNCTION
void DrawMap(const Map &map)
{
    // I don't really get what this is supposed to do.
}

Room DrunkardsWalk(int id, int width, int height, int iterations)
{
    Room room = CreateRoom(id, width, height, RoomType::FightRoom);
    bool hasFoundAOuterWall = false;
    for (int i = 0; i < width * height; i++)
    {
        room.tiles[i].walkable = false;
    }

    for (int i = 0; i < iterations; i++)
    {
        int posX = (int)(width / 2);
        int posY = (int)(height / 2);

        for (int i = 0; i < width * 2; i++)
        {
            int random = rand();
            switch (random % 4)
            {
            case 0: // Up
                posY--;
                break;
            case 1: // Down
                posY++;
                break;
            case 2: // Left
                posX--;
                break;
            case 3: // Right
                posX++;
                break;
            default:
                // How did we get here?
                break;
            }
            // Out of bounds check
            if (posX < 0)
            {
                posX++;
                hasFoundAOuterWall = true;
            }
            if (posX >= width)
            {
                posX--;
                hasFoundAOuterWall = true;
            }
            if (posY < 0)
            {
                posY++;
                hasFoundAOuterWall = true;
            }
            if (posY >= height)
            {
                posY--;
                hasFoundAOuterWall = true;
            }

            // Set the tile to walkable
            room.tiles[posX + posY * width].walkable = true;
        }
        // Check if we have found an outer wall, if not, we repeat the iteration until we do.
        if (i == iterations - 1 && !hasFoundAOuterWall)
        {
            i--;
        }
    }
}

void RoomDraw(Room *room)
{
    for (int x = 0; x < room->width; x++)
    {
        for (int y = 0; y < room->height; y++)
        {
            if (!GetTile(room, x, y).walkable)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, BLUE);
            }
        }
    }
}

// TEMPORARY FUNCTION
Tile GetTile(Room *room, int x, int y)
{
    Tile tile;
    if (x < 1 || x >= room->width - 1 || y < 1 || y >= room->height - 1)
    {
        tile.walkable = false;
    }
    else
    {
        tile.walkable = true;
    }
    return tile;
}