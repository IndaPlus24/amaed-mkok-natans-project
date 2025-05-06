#ifndef ATTACK_F_H
#define ATTACK_F_H

#include "attackStruct.h"
#include "gameData.h"

// Does as described on the tin
Attack CreateAttack(void *owner, AttackType attackType);
/// @brief Does as described on the tin
/// @param attack The attack
/// @param timestamp Time since the attack started
/// @param pos The position of the attacking entity
/// @param dir The direction to attack in
/// @param gameData Game Data
void AttackUpdate(Attack *attack, float timestamp, Vector2 pos, Vector2 dir, GameData *gameData);

/// @brief Does as described
/// @param attack The attack
/// @param pos The position of the attacking entity
/// @param dir The direction to attack in
void AttackDebugDraw(const Attack *attack, Vector2 pos, Vector2 dir);

// Ends the attack immediately: freeing all memory related to it
void AttackForceEnd(Attack *attack);

#endif