#ifndef ATTACK_F_H
#define ATTACK_F_H

#include "attackStruct.h"
#include "gameData.h"

Attack CreateAttack(void *owner, AttackType attackType);
void AttackUpdate(Attack *attack, float timestamp, Vector2 pos, Vector2 dir, GameData *gameData);
void AttackDebugDraw(const Attack *attack, Vector2 pos, Vector2 dir);

#endif