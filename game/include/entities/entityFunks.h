#ifndef ENTITY_F_H
#define ENTITY_F_H

#include <unordered_set>

#include "gameData.h"
#include "collisionLayers.h"

/// @brief Move an entity pos by adding move without passing through
/// @param pos Pointer to entity's position vector
/// @param move The vector to add to pos
/// @param width Collider width (x-axis)
/// @param height Collider height (y-axis)
/// @param gameData Game data
/// @return true if it collided with a wall
bool EntityMove(Vector2 *pos, Vector2 move, int width, int height, GameData *gameData);
// Maybe it should take the current room rather than the entire gameData

/// @brief Get a list of void pointers to the entities that you want within an area
/// @param pos The position of the CENTRE of the box
/// @param width
/// @param height
/// @param layers
/// @param gameData
/// @param self
/// @return
std::unordered_set<void *> GetEntitiesInArea(Vector2 pos, int width, int height, CollisionLayers layers, const GameData *gameData, const void *self);

CollisionLayers GetEntityType(const void *entity, const GameData *gameData);

void AttackEntity(void *entity, void *source, GameData *gameData, float damage, Vector2 force);

Vector2 GetEntityPosition(const void *entity, const GameData *gameData);



#endif