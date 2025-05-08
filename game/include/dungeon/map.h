#ifndef MAP_H
#define MAP_H

struct Room;
struct Door;

#include <vector>
#include "tile.h"

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
    std::vector<int> tags;
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
};

// ---------------------
// Map Contains Rooms and doors
// ---------------------

struct Map
{
    std::vector<Room> rooms;
    std::vector<Door> doors;
};

Room CreateRoom(int id, int width, int height);

Room DrunkardsWalk(int startX, int startY, int id, int width, int height, int iterations, Door* previousDoor = nullptr);

void ConnectRooms(Door* door1, Door* door2);

Door CreateDoor(int fromId, int toId);

void DrawMap(const Map &map);

void RoomDraw(Room *room);

Tile GetTile(Room *room, int x, int y);

#endif