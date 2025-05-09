#ifndef WEAPONS_F_H
#define WEAPONS_F_H

#include "weaponsStruct.h"
#include "gameData.h"

enum class WeaponPrefabs {
    Purju,
};

Weapon CreateWeapon(WeaponPrefabs prefab);

Attack CreateAttack(void *owner, const Weapon * weapon);

void WeaponDraw(const Weapon *weapon, Vector2 position, int dir, int frame, WeaponState state);

#endif