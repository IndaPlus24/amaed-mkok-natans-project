#include "raylib.h"

#include "map.h"

// TEMPORARY FUNCTION
Room CreateRoom(int id, int width, int height)
{
    Room room;
    room.id = id;
    room.height = height;
    room.width = width;
    // room.type;
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

void RoomDraw(Room *room)
{
    for (int x = 0; x < room->width; x++)
    {
        for (int y = 0; y < room->height; y++)
        {
            if (!GetTile(room, x, y).isWalkable)
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
    if (x < 1 || x >= room->width -1 || y < 1 || y >= room->height - 1)
    {
        tile.isWalkable = false;
    }
    else
    {
        tile.isWalkable = true;
    }
    return tile;
}