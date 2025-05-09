#ifndef WEAPONS_S_H
#define WEAPONS_S_H

#include "attackStruct.h" 
#include "spriteSheet.h"

enum class WeaponState {
    Idle,
    Walk,
    Attack,
};

typedef struct Weapon {
    AttackType type;

    SpriteSheet idleSheet;
    SpriteSheet walkSheet;
    SpriteSheet attackSheet;

    float damageMult;
    float forceMult;
    float animationSpeedMult;
} Weapon;

#endif