#ifndef MAP_H
#define MAP_H

struct Room;
struct Door;

#include "enemiesStruct.h"
#include <vector>
#include "tile.h"

struct GameData;          // forward declaration replaces the include


// ---------------------
// Room
// ---------------------

// ----------------------------
// Room Types:
//
// 1. Corridor - Width or Height not wider than 2
// 2. Fight Room - Few Obstacles
// 3. Puzzle Room - More Obstacles and NPCs
// 4. Boss Room - Not Fully Procedural (Predrawn) - Can have procedural elements
enum class RoomType
{
    Corridor,
    FightRoom,
    PuzzleRoom,
    BossRoom
};

/// @brief  first door is the entrance so we always know that the room we came from is the first one.
struct Room
{
    int id;
    int width, height;
    RoomType type;
    Tile* tiles;
    Door* doors; // Doors in the room
    int doorsCount; // Number of doors in the room
    // Could add tags and such later...
};

// ---------------------
// Door: Connects 2 rooms.
// ---------------------

struct Door
{
    int fromDir;
    int posX;
    int posY;
    int fromRoomId;
    int toRoomId;
    Door* linkedDoor; // Pointer to the linked door in the other room.
    Room* room; // Pointer to the room this door is in.
};

// ---------------------
// Map Contains Rooms and doors
// ---------------------

struct Map
{
    Room * rooms; // Pointer to the array of rooms
    Enemies* enemies; // Pointer to the enemies in all rooms.
    int roomsPerFloor;
    int floors;
    int currentFloor;
    int currentRoom;
};

struct Chamber
{
    int x, y, w, h;          // bounding rectangle of this Chamber
    Chamber *left  = nullptr;   // children after a split (nullptr when this is a final Chamber)
    Chamber *right = nullptr;
    int roomX = 0, roomY = 0, roomW = 0, roomH = 0;
};

Room CreateRoom(int id, int width, int height);

Map CreateMap(int floors, int roomsPerFloor, int width, int height, int floorSwitch, GameData* gameData);

Room DrunkardsWalk(int startX, int startY, int id, int width, int height, int iterations, Door *previousDoor, int entryDoorX, int entryDoorY);

Room BSP(int id, int width, int height, int iterations, Door *previousDoor);

void ConnectRooms(Door* door1, Door* door2);

Door CreateDoor(int fromId, int toId);

void DrawMap(const Map &map);

void RoomDraw(Room *room);

Tile GetTile(Room *room, int x, int y);

#endif