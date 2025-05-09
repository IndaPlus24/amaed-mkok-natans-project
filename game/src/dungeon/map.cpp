#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include "map.h"
#include "enemiesFunks.h"
#include "gameData.h"

void ConnectRooms(Door *door1, Door *door2)
{
    if (door1 == nullptr || door2 == nullptr)
    {
        printf("One of the doors is null\n");
        return;
    }
    printf("Connecting doors %d and %d\n", door1->fromRoomId, door2->fromRoomId);
    // connect the two doors
    door1->linkedDoor = door2;
    door2->linkedDoor = door1;
    // links room ids
    door1->toRoomId = door2->fromRoomId;
    door2->toRoomId = door1->fromRoomId;
};

Door CreateDoor(int roomId, int posX, int posY)
{
    Door door;
    door.fromRoomId = roomId;
    door.posX = posX;
    door.posY = posY;

    return door;
};
// TEMPORARY FUNCTION
void DrawMap(const Map &map) {
    // I don't really get what this is supposed to do.
    // I think it is suppose to do a _map_. You know, like the piece of paper. I could be wrong though. - N
};

Map CreateMap(int floors, int roomsPerFloor, int width, int height, int floorSwitch, GameData *gameData)
{
    Map map = {};
    map.floors = floors;
    map.roomsPerFloor = roomsPerFloor;
    Door *previousDoor = nullptr;
    map.rooms = new Room[floors * roomsPerFloor];
    for (int i = 0; i < floors; i++)
    {
        for (int j = 0; j < roomsPerFloor; j++)
        {
            if (i >= floorSwitch)
            {
                int entryX = previousDoor ? previousDoor->posX : width / 2;
                int entryY = previousDoor ? previousDoor->posY : height / 2;
                if (previousDoor)
                {
                    if (previousDoor->posX < 0)
                    {
                        entryX = width - 2;
                    }
                    else if (previousDoor->posX >= width)
                    {
                        entryX = 1;
                    }
                    if (previousDoor->posY < 0)
                    {
                        entryY = height - 2;
                    }
                    else if (previousDoor->posY >= height)
                    {
                        entryY = 1;
                    }
                }
                printf("Creating room using Drunkard %d on floor %d with entry door at (%d, %d)\n", i * roomsPerFloor + j, i, entryX, entryY);
                map.rooms[i * roomsPerFloor + j] = DrunkardsWalk(entryX, entryY, i * roomsPerFloor + j, width, height, 100, previousDoor, entryX, entryY);
            }
            else
            {
                printf("Creating room using BSP %d on floor %d\n", i * roomsPerFloor + j, i);
                map.rooms[i * roomsPerFloor + j] = BSP(i * roomsPerFloor + j, width, height, 100, previousDoor);
            }
            previousDoor = &map.rooms[i * roomsPerFloor + j].doors[1];
        }
    }
    map.currentRoom = 0;
    map.currentFloor = 0;
    printf("Map created with %d floors and %d rooms per floor\n", map.floors, map.roomsPerFloor);
    return map;
}

Room CreateRoom(int id, int width, int height, RoomType type)
{
    Room room;
    room.id = id;
    room.height = height;
    room.width = width;
    room.type = type;
    room.tiles = (Tile *)malloc(sizeof(Tile) * width * height);
    room.doors = (Door *)malloc(sizeof(Door) * 2); // 2 doors max for now
    for (int i = 0; i < width * height; i++)
    {
        room.tiles[i].walkable = false;
        room.tiles[i].door = nullptr;
    }
    return room;
}

Room DrunkardsWalk(int startX, int startY, int id, int width, int height, int iterations, Door *previousDoor, int entryDoorX, int entryDoorY)
{
    int possibleDoors[25][2]; // 100 first locations visited by the drunkard that are guaranteed to be walkable. 0 = Up, 1 = Down, 2 = Left, 3 = Right
    int possibleDoorsCount = 0;

    Room room = CreateRoom(id, width, height, RoomType::FightRoom);
    bool hasFoundAOuterWall = false;

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
            if (posX < 1)
            {
                if (possibleDoorsCount < 25)
                {
                    possibleDoors[possibleDoorsCount][0] = posX;
                    possibleDoors[possibleDoorsCount][1] = posY;
                    possibleDoorsCount++;
                    hasFoundAOuterWall = true;
                }
                posX++;
            }
            if (posX >= width - 1)
            {
                if (possibleDoorsCount < 25)
                {
                    possibleDoors[possibleDoorsCount][0] = posX;
                    possibleDoors[possibleDoorsCount][1] = posY;
                    possibleDoorsCount++;
                    hasFoundAOuterWall = true;
                }
                posX--;
            }
            if (posY < 1)
            {
                if (possibleDoorsCount < 25)
                {
                    possibleDoors[possibleDoorsCount][0] = posX;
                    possibleDoors[possibleDoorsCount][1] = posY;
                    possibleDoorsCount++;
                    hasFoundAOuterWall = true;
                }
                posY++;
            }
            if (posY >= height - 1)
            {
                if (possibleDoorsCount < 25)
                {
                    possibleDoors[possibleDoorsCount][0] = posX;
                    possibleDoors[possibleDoorsCount][1] = posY;
                    possibleDoorsCount++;
                    hasFoundAOuterWall = true;
                }
                posY--;
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
    }
    if (previousDoor != nullptr) // this should only happen if the room is the first one, but in the case that it isn't it shouldn't crash, i hope.
    {
        // Connect the doors

        room.doors[0] = CreateDoor(id, entryDoorX, entryDoorY);

        if (previousDoor)
        {
            ConnectRooms(previousDoor, &room.doors[0]);
        }
        room.tiles[room.doors[0].posX + room.doors[0].posY * width].door = &room.doors[0];
        room.tiles[room.doors[0].posX + room.doors[0].posY * width].walkable = true;
    }

    room.doors[1] = CreateDoor(id, possibleDoors[random() % possibleDoorsCount][0], possibleDoors[random() % possibleDoorsCount][1]); // door index 1 is the exit, linmking it to the previous room is fixed if there is a next one since that function does the same as this one
    room.tiles[room.doors[1].posX + room.doors[1].posY * width].door = &room.doors[1];
    room.tiles[room.doors[1].posX + room.doors[1].posY * width].walkable = true;

    if (previousDoor == nullptr)
    {
        room.doors[0] = room.doors[1];
        room.doors[0].linkedDoor = &room.doors[1];
        int t = room.doors[0].posX + room.doors[0].posY * room.width;
        room.tiles[t].door = &room.doors[0];
    }
    return room;
}

// BSP algorithm
// Written with pseduocode generated by ChatGPT

static void carveChamber(Room *room, int x, int y, int w, int h)
{
    for (int iy = y; iy < y + h; ++iy)
        for (int ix = x; ix < x + w; ++ix)
            room->tiles[ix + iy * room->width].walkable = true;
}

// “L” shaped connector (first horizontal, then vertical or vice-versa)
static void carveCorridor(Room *room, int x1, int y1, int x2, int y2)
{
    int corridorX = x1, corridorY = y1;
    while (corridorX != x2)
    {
        corridorX += (x2 > corridorX) ? 1 : -1;
        room->tiles[corridorX + corridorY * room->width].walkable = true;
    }
    while (corridorY != y2)
    {
        corridorY += (y2 > corridorY) ? 1 : -1;
        room->tiles[corridorX + corridorY * room->width].walkable = true;
    }
}

static void splitChamber(Chamber *chamber, std::vector<Chamber *> &outChambers, int minChamber, int &remainingSplits)
{
    if (remainingSplits <= 0)
    {
        outChambers.push_back(chamber);
        return;
    }

    bool splitHoriz = (rand() % 2) == 0;
    if (chamber->w > chamber->h && chamber->w / chamber->h >= 1.25f)
        splitHoriz = false; // favour vertical split
    else if (chamber->h > chamber->w && chamber->h / chamber->w >= 1.25f)
        splitHoriz = true; // favour horizontal split

    int max = (splitHoriz ? chamber->h : chamber->w) - minChamber;
    if (max <= minChamber)
    {
        outChambers.push_back(chamber);
        return;
    }

    int split = rand() % (max - minChamber) + minChamber;

    if (splitHoriz) // horizontal (i.e. we cut *across* the Y axis)
    {
        chamber->left = new Chamber{chamber->x, chamber->y, chamber->w, split};
        chamber->right = new Chamber{chamber->x, chamber->y + split, chamber->w, chamber->h - split};
    }
    else // vertical split
    {
        chamber->left = new Chamber{chamber->x, chamber->y, split, chamber->h};
        chamber->right = new Chamber{chamber->x + split, chamber->y, chamber->w - split, chamber->h};
    }
    --remainingSplits;

    splitChamber(chamber->left, outChambers, minChamber, remainingSplits);
    splitChamber(chamber->right, outChambers, minChamber, remainingSplits);
}

static void freeChamberTree(Chamber *chamber)
{
    if (!chamber)
        return;
    freeChamberTree(chamber->left);
    freeChamberTree(chamber->right);
    delete chamber;
}

// Main BSP function
Room BSP(int id, int width, int height, int iterations, Door *previousDoor)
{
    const int MIN_CHAMBER_SIZE = 6; // won’t split below this size

    // create empty room (= whole dungeon)
    Room room = CreateRoom(id, width, height, RoomType::FightRoom);

    // build BSP tree
    Chamber *root = new Chamber{0, 0, width, height};
    std::vector<Chamber *> chambers;
    int remainingSplits = iterations;
    splitChamber(root, chambers, MIN_CHAMBER_SIZE, remainingSplits);

    for (Chamber *l : chambers)
    {
        const int margin = 1;                          // keep 1-tile buffer to walls
        int rw = rand() % (l->w - 2 * margin - 3) + 3; // at least 3 × 3
        int rh = rand() % (l->h - 2 * margin - 3) + 3;
        int rx = l->x + rand() % (l->w - rw - 2 * margin) + margin;
        int ry = l->y + rand() % (l->h - rh - 2 * margin) + margin;

        l->roomX = rx;
        l->roomY = ry;
        l->roomW = rw;
        l->roomH = rh;
        carveChamber(&room, rx, ry, rw, rh);
    }

    for (size_t i = 1; i < chambers.size(); ++i)
    {
        // pick a random point inside each room
        Chamber *a = chambers[i - 1];
        Chamber *b = chambers[i];
        int ax = a->roomX + rand() % a->roomW;
        int ay = a->roomY + rand() % a->roomH;
        int bx = b->roomX + rand() % b->roomW;
        int by = b->roomY + rand() % b->roomH;
        carveCorridor(&room, ax, ay, bx, by);
    }

    int possibleDoors[25][2];
    int possibleDoorsCount = 0;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
        {
            if (!room.tiles[x + y * width].walkable)
                continue;
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
            {
                if (possibleDoorsCount < 25)
                {
                    possibleDoors[possibleDoorsCount][0] = x;
                    possibleDoors[possibleDoorsCount][1] = y;
                    ++possibleDoorsCount;
                }
            }
        }

    if (previousDoor != nullptr)
    {
        // entry door is where we came from
        room.doors[0] = CreateDoor(id, previousDoor->posX, previousDoor->posY);
        if (previousDoor)
        {
            ConnectRooms(previousDoor, &room.doors[0]);
        }
        int idx = room.doors[0].posX + room.doors[0].posY * width;
        room.tiles[idx].door = &room.doors[0];
        room.tiles[idx].walkable = true;
    }

    // exit door – any random perimeter walkable tile
    if (possibleDoorsCount == 0)
    {
        // extremely unlikely – fallback: dead-centre of first room
        Chamber *chamber = chambers[0];
        int dx = chamber->roomX + chamber->roomW / 2;
        int dy = chamber->roomY + chamber->roomH / 2;
        possibleDoors[0][0] = dx;
        possibleDoors[0][1] = dy;
        possibleDoorsCount = 1;
    }

    int pick = rand() % possibleDoorsCount;
    int dx = possibleDoors[pick][0];
    int dy = possibleDoors[pick][1];

    room.doors[1] = CreateDoor(id, dx, dy);
    room.tiles[room.doors[1].posX + room.doors[1].posY * width].door = &room.doors[1];
    room.tiles[room.doors[1].posX + room.doors[1].posY * width].walkable = true;

    if (previousDoor == nullptr)
    {
        room.doors[0] = room.doors[1];
        room.doors[0].linkedDoor = &room.doors[1];
        int t = room.doors[0].posX + room.doors[0].posY * room.width;
        room.tiles[t].door = &room.doors[0];
    }

    printf("trying to free the chamber tree\n");
    freeChamberTree(root);
    printf("freed the chamber tree\n");
    return room;
}

// End of code written based on ChatGPT's pseudocode

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

    Tile tile = room->tiles[x + y * room->width];

    return tile;
}
