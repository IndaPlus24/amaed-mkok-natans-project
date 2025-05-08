#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "enemiesFunks.h"


// TEMPORARY FUNCTION
Room CreateRoom(int id, int width, int height, RoomType type)
{
    Room room;
    room.id = id;
    room.height = height;
    room.width = width;
    room.type = type;
    // room.tags;
    room.tiles = (Tile *)malloc(sizeof(Tile) * width * height);
    room.doors = (Door *)malloc(sizeof(Door) * 2); // 2 doors max for now
    return room;
}

// TEMPORARY FUNCTION
Door CreateDoor(Door* fromDoor, int roomId, int posX, int posY)
{
    Door door;
    door.fromRoomId = roomId;
    door.posX = posX;
    door.posY = posY;
    ConnectRooms(fromDoor, &door);
    return door;
};
void ConnectRooms(Door* door1, Door* door2)
{
    // connect the two doors
    door1->linkedDoor = door2;
    door2->linkedDoor = door1;
    // links room ids
    door1->toRoomId = door2->fromRoomId;
    door2->toRoomId = door1->fromRoomId;
}
// TEMPORARY FUNCTION
void DrawMap(const Map &map)
{
    // I don't really get what this is supposed to do.
    // I think it is suppose to do a _map_. You know, like the piece of paper. I could be wrong though. - N
}

Room* CreateMap(int floors, int roomsPerFloor, int width, int height, int floorSwitch)
{
    Room *map = (Room *)malloc(sizeof(Room) * floors * roomsPerFloor);
    for (int i = 0; i < floors; i++)
    {
        for (int j = 0; j < roomsPerFloor; j++)
        {
            if(i >= floorSwitch)
            {
                map[i * roomsPerFloor + j] = DrunkardsWalk(i * roomsPerFloor + j, width, height, 15, map[i * roomsPerFloor + j - 1].doors[1]);
            }
            else
            {
                map[i * roomsPerFloor + j] = CreateRoom(i * roomsPerFloor + j, width, height, RoomType::FightRoom);
            }
        }
        
    }
    
    return map;
}

Room DrunkardsWalk(int startX, int startY, int id, int width, int height, int iterations, Door* previousDoor = nullptr)
{
    int possibleDoors[25][2][4]; // 100 first locations visited by the drunkard that are guaranteed to be walkable. 0 = Up, 1 = Down, 2 = Left, 3 = Right
    int possibleDoorsCount = 0;
    Room room = CreateRoom(id, width, height, RoomType::FightRoom);
    bool hasFoundAOuterWall = false;
    for (int i = 0; i < width * height; i++)
    {
        room.tiles[i].walkable = false;
    }

    for (int i = 0; i < iterations; i++)
    {
        int posX = startX;
        int posY = startY;

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
                possibleDoors[possibleDoorsCount][0][2] = posX;
                possibleDoors[possibleDoorsCount][1][2] = posY;
                possibleDoorsCount++;
                hasFoundAOuterWall = true;
            }
            if (posX >= width)
            {
                posX--;
                possibleDoors[possibleDoorsCount][0][3] = posX;
                possibleDoors[possibleDoorsCount][1][3] = posY;
                possibleDoorsCount++;
                hasFoundAOuterWall = true;
            }
            if (posY < 0)
            {
                posY++;
                possibleDoors[possibleDoorsCount][0][1] = posX;
                possibleDoors[possibleDoorsCount][1][1] = posY;
                possibleDoorsCount++;
                hasFoundAOuterWall = true;
            }
            if (posY >= height)
            {
                posY--;
                possibleDoors[possibleDoorsCount][0][0] = posX;
                possibleDoors[possibleDoorsCount][1][0] = posY;
                possibleDoorsCount++;
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
        // we have found an outer wall, we can add a door to the room.
        roomms.doors[0] = CreateDoor(previousDoor, id, possibleDoors[random()%possibleDoors][0][random()%4], possibleDoors[random()%possibleDoors][1][random()%4]);
        rooms.doors[1] = CreateDoor(previousDoor, id, possibleDoors[random()%possibleDoors][0][random()%4], possibleDoors[random()%possibleDoors][1][random()%4]); // door index 1 is the exit, linmking it to the previous room is fixed if there is a next one since that function does the same as this one
    }
    return room;
}
// Creates a room using the BSP algorithm
Room BSP(int id, int width, int height, int iterations, Door* previousDoor = nullptr)
{
    Room room = CreateRoom(id, width, height, RoomType::FightRoom);
    for (int i = 0; i < width * height; i++)
    {
        room.tiles[i].walkable = false;
    }

    for (int i = 0; i < iterations; i++)
    {

    }
        

    return room;
}
void BSPSpliterFunction(int minChamberWidth, int minChamberHeight, Room* room){
    if (rand() % 2 == 0) // Horizontal split
    {
        int splitX = rand() % (room->width - minChamberWidth) + minChamberWidth;
        int splitY = rand() % room->height;
        for (int i = 0; i < room->width; i++)
        {
            room->tiles[i + splitY * room->width].walkable = false;
        }
    }
    else // Vertical split
    {
        int splitX = rand() % room->width;
        int splitY = rand() % (room->height - minChamberHeight) + minChamberHeight;
        for (int i = 0; i < room->height; i++)
        {
            room->tiles[splitX + i * room->width].walkable = false;
        }
    }
}

void RoomDraw(Room *room)
{
    for (int x = 0; x < room->width; x++)
    {
        for (int y = 0; y < room->height; y++)
        {
            if (!GetTile(r  oom, x, y).walkable)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, BLUE);
            }
            if (GetTile(room, x, y).door != nullptr)
            {
                DrawRectangle(x * tileSize, y * tileSize, tileSize, tileSize, RED);
            } 
        }
    }
}

// TEMPORARY FUNCTION
Tile GetTile(Room *room, int x, int y)
{
    // Make sure no one thinks you can walk across the tiles outside.
    if (x < 0 || x >= room->width || y < 0 || y >= room->height)
    {
        Tile tile;
        tile.walkable = false;
        tile.isDoor = false;
        tile.isWall = true;
        
        return tile;
    }

    Tile tile = room->tiles[x + y * room->width];

    
    return tile;
}
