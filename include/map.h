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
enum class RoomType {
    Corridor,
    FightRoom,
    PuzzleRoom,
    BossRoom
};

struct Room
{
    int id; 
    int width, height;
    RoomType type; 
    std::vector<int> tags;
    std::vector<Tile> tiles;
    //Could add tags and such later...
};


// ---------------------
// Door: Connects 2 rooms. 
// ---------------------

struct Door {
    int fromDir;
    int fromRoomOffset;
    int toRoomOffset;
    int fromRoomId;
    int toRoomId;
};

// ---------------------
// Map Contains Rooms and doors
// ---------------------

struct Map
{
    std::vector<Room> rooms;
    std::vector<Door> doors;
};

Room createRoom(int id, int width, int height);

Door createDoor(int fromId, int toId);

void drawMap(const Map& map);

#endif