#ifndef PROJECTILES_F_H
#define PROJECTILES_F_H

#include "gameData.h"

// Behold: the lifecycle of a projectile

Projectile *CreateProjectile(ProjectilePrefabs prefab, GameData *gameData, void *owner, Vector2 direction, float damage, float force);

void ProjectileUpdate(Projectile *projectile, GameData *gameData);

void DestroyProjectile(Projectile *Projectile, GameData *gameData);

void ProjectileDebugDraw(const Projectile *projectile);

#endif