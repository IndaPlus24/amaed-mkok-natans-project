#include "weaponsFunks.h"
#include "attackFunks.h"

Weapon CreateWeapon(WeaponPrefabs prefab)
{
    Weapon weapon;

    switch (prefab)
    {
    case WeaponPrefabs::Purju:
        weapon.type = AttackType::testMelee;
        weapon.damageMult = 1.0f;
        weapon.forceMult = 1.0f;
        weapon.animationSpeedMult = 1.0f;

        // Temporarily just load all of them
        weapon.idleSheet = LoadSpriteSheet("assets/sprites/purju_idle.png", 8, 1);
        weapon.walkSheet = LoadSpriteSheet("assets/sprites/purju_idle.png", 8, 1);
        weapon.attackSheet = LoadSpriteSheet("assets/sprites/purju_attack.png", 8, 3);
        break;
    }

    return weapon;
}

Attack CreateAttack(void *owner, const Weapon *weapon)
{
    Attack attack = CreateAttack(owner, weapon->type);

    attack.damage *= weapon->damageMult;
    attack.force *= weapon->forceMult;

    for (int i = 0; i < attack.keyFrameCount; i++)
    {
        attack.keyFrames[i].time *= weapon->animationSpeedMult;
    }

    return attack;
}

void WeaponDraw(const Weapon *weapon, Vector2 position, int dir, int frame, WeaponState state)
{
    switch (state)
    {
    case WeaponState::Idle:
        DrawCentre(&weapon->idleSheet, dir, frame, position);
        break;
    case WeaponState::Walk:
        DrawCentre(&weapon->walkSheet, dir, frame, position);
        break;
    case WeaponState::Attack:
        DrawCentre(&weapon->attackSheet, dir, frame, position);
        break;
    }
}
