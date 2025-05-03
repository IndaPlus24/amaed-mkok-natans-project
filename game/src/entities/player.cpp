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
#include "playerFunks.h"
#include "entityFunks.h"

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

    p->velocity = Vector2Add(p->velocity, Vector2Scale(dir, p->acceleration * GetFrameTime()));

    if (Vector2Length(p->velocity) > p->speed) p->velocity = Vector2Scale(Vector2Normalize(p->velocity), p->speed);
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

void PlayerUpdate(GameData *gameData,const Inputs *in)
{
    Player *player = &gameData->player;
    Enemies *enemies = &gameData->enemies;
    Projectiles *projectiles = &gameData->projectiles;
    if (!p->alive)
        return;

    if (p->stunTimer > 0.0f) /* still stunned */
    {
        p->stunTimer -= GetFrameTime();
        return;
    }
      
    Player *player = &gameData->player;

    switch (player->state)
    {
    case PlayerState::Neutral:
        PlayerMovement(player, in);
        break;

    default:
        break;
    }

    Vector2 move = Vector2Scale(player->velocity, GetFrameTime());
    EntityMove(&player->position, move, player->width, player->height, gameData);
    
    p->attackCooldownTimer += GetFrameTime();
    PlayerAttack(p, pool, enemies, in); /* pass enemies along */
}
  
  void PlayerDraw(Player *player)
{
    DrawRectangle((int)(player->position.x - ((player->width + 1) >> 1)), (int)(player->position.y - ((player->height + 1) >> 1)), (int)player->width, (int)player->height, GREEN);

    // A "simple" "algoritm" to translate a vector into an integer representing its angle (clockwise is positive, (0,1) is 0)
    int dir = 0;
    if (player->direction.x != 0)
    {
        int a = player->direction.x > 0 ? -1 : 1;
        int b = 1;
        if (player->direction.y == 0)
        {
            b = 2;
        }
        else if (player->direction.y < 0)
        {
            b = 3;
        }
        dir = (8 + a * b) % 8;
    }
    else if (player->direction.y < 0)
    {
        dir = 4;
    }
    DrawCentre(&player->sheets[0], dir, 0, player->position);

// THIS FUNCTION WAS CREATED BY COPILOT, I DID NOT WRITE IT, IT IS ONLY HERE FOR TESTING PURPOSES
// TODO : MAKE SURE THIS FUNCTION IS WRITTEN BY A HUMAN.
Player CreatePlayer(Vector2 spawnPos, PlayerWeaponType type){ 
    Player player = {0};
    player.position = spawnPos;
    player.type = type;
    player.speed = 2.0f;
    player.acceleration = 1.0f;
    player.health = 250.0f;
    player.attackRange = 12.0f;
    player.attackDamage = 25.0f;
    player.attackCooldown = 0.2f;
    player.attackCooldownTimer = 0.0f;
    player.projectileSpeed = 500.0f;
    player.stunTimer = 0.0f;
    player.alive = true;
    player.width = 14;
    player.height = 15;

    return player;
}