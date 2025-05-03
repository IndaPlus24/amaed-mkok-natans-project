#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>        
#include <stdbool.h>
#include "input.h"     

/*────────────────── Types ──────────────────────────*/
typedef enum {
    PLAYER_MELEE,
    PLAYER_RANGED
} PlayerWeaponType;

/* Main runtime state for a single player */
typedef struct Player {
    /* kinematics */
    Vector2 position;
    Vector2 velocity;
    Vector2 direction;

    float   speed;
    float   acceleration;
    float   health;
    float   attackRange;
    float   attackDamage;
    float   attackCooldown;
    float   attackCooldownTimer;
    float   projectileSpeed;
    float   stunTimer;

    bool    alive;
    bool    swinging;             /* true while a melee swing is active */

    PlayerWeaponType type;
} Player;

struct ProjectilePool;
struct Enemies;

void updatePlayer(Player *p, const Inputs *in, struct ProjectilePool *pool, struct Enemies *enemies);

Player CreatePlayer(Vector2 spawnPos, PlayerType type);
#endif
#endif /* PLAYER_H */
