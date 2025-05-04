#ifndef PROJECTILES_S_H
#define PROJECTILES_S_H

// TODO: Go back to attack.cpp and fix the SpawnProjectiles case
#include <raylib.h>
#include "attackStruct.h"

enum class ProjectilePrefabs {
    Default,
};

enum class ProjectileBehaviour {
    Straight, // Just go in a straight line
    // Homing, // Home in a valid target
};


typedef struct Projectile {
    void *owner; // The thing that shot the projectile
    Vector2 position;
    float speed;
    Vector2 direction;

    CollisionLayers targets;
    ProjectileBehaviour behaviour;

    int pierces;
    int bounces;
    float damage;
    float force;

    HitBox *hitBoxes;
    int hitBoxCount;

    std::unordered_set<void *> *hits;  // Make sure we don't hit the same target multiple times (unless we want to)

} Projectile;

typedef struct Projectiles {
    Projectile *list;
    int count;
    int capacity;
} Projectiles;

#endif