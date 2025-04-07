#ifndef MAP_H
#define MAP_H

struct Room;
struct Door;


#include <vector>
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
struct Room
{
    int id; 
    int width, height;
    int type; 
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
// Map
// ---------------------

struct Map
{
    std::vector<Room> rooms;
    std::vector<Door> doors;
};

Room createRoom(int id, int width, int height);

Door createDoor(int fromId, int toId);

#endif