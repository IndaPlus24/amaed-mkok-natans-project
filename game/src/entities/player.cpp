#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#include "player.h"
#include "enemies.h"
#include "input.h"

void PlayerMovement(Player *p, const Inputs *in)
{
    Vector2 dir = {0};

    if (in->up == Pressed || in->up == Down)
        dir.y -= 1.0f;
    if (in->down == Pressed || in->down == Down)
        dir.y += 1.0f;
    if (in->left == Pressed || in->left == Down)
        dir.x -= 1.0f;
    if (in->right == Pressed || in->right == Down)
        dir.x += 1.0f;

    /* decelerate when no key is held */
    if (Vector2Length(dir) == 0.0f)
    {
        p->velocity = Vector2Lerp(p->velocity, (Vector2){0, 0}, 8.0f * GetFrameTime());
        p->direction = Vector2Normalize(p->velocity);
        return;
    }

    dir = Vector2Normalize(dir);
    p->direction = dir;

    p->velocity = Vector2Add(p->velocity,
                             Vector2Scale(dir, p->acceleration * GetFrameTime()));

    if (Vector2Length(p->velocity) > p->speed)
        p->velocity = Vector2Scale(Vector2Normalize(p->velocity), p->speed);

    p->position = Vector2Add(p->position,
                             Vector2Scale(p->velocity, GetFrameTime()));
}
void PlayerAttack(Player *p, Enemies *enemies, const Inputs *in)
{
    bool fire = (in->a == Pressed || in->a == Down); /* hold (A) or tap */

    if (!fire || p->attackCooldownTimer < p->attackCooldown)
        return;

    switch (p->type)
    {
    case PLAYER_MELEE:
    {
        /* loop over every enemy and apply damage if inside range */
        for (int i = 0; i < enemies->count; ++i)
        {
            Enemy *e = &enemies->enemies[i];

            if (!e->alive)
                continue;

            float dist = Vector2Distance(p->position, e->position);

            if (dist <= p->attackRange)
            {
                e->health -= p->attackDamage;

                if (e->health <= 0)
                    e->alive = false;
            }
        }
    }
    break;
    case PLAYER_RANGED:
        // Handle ranged attack
        break;
    }

    p->attackCooldownTimer = 0.0f;
}

void PlayerUpdate(Player *p,ProjectilePool *pool,Enemies *enemies,const Inputs *in)
{
    if (!p->alive)
        return;

    if (p->stunTimer > 0.0f) /* still stunned */
    {
        p->stunTimer -= GetFrameTime();
        return;
    }

    PlayerMovement(p, in);

    p->attackCooldownTimer += GetFrameTime();
    PlayerAttack(p, pool, enemies, in); /* pass enemies along */
}

// THIS FUNCTION WAS CREATED BY COPILOT, I DID NOT WRITE IT, IT IS ONLY HERE FOR TESTING PURPOSES
// TODO : MAKE SURE THIS FUNCTION IS WRITTEN BY A HUMAN.
Player CreatePlayer(Vector2 spawnPos, PlayerType type){ 
    Player p = {0};
    p.position = spawnPos;
    p.type = type;
    p.speed = 200.0f;
    p.acceleration = 1000.0f;
    p.health = 100.0f;
    p.attackRange = 50.0f;
    p.attackDamage = 10.0f;
    p.attackCooldown = 1.0f;
    p.attackCooldownTimer = 0.0f;
    p.projectileSpeed = 500.0f;
    p.stunTimer = 0.0f;
    p.alive = true;

    return p;
}