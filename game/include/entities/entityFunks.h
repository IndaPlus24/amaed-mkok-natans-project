#ifndef ENTITY_F_H
#define ENTITY_F_H
#include "gameData.h"

/// @brief Move an entity pos by adding move without passing through
/// @param pos Pointer to entity's position vector
/// @param move The vector to add to pos
/// @param width Collider width (x-axis)
/// @param height Collider height (y-axis)
/// @param gameData Game data
/// @return 
bool EntityMove(Vector2 *pos, Vector2 move, int width, int height, GameData *gameData);
// Maybe it should take the current room rather than the entire gameData

#endif