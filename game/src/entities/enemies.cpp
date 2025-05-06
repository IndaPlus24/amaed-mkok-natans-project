#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#include "gameData.h"
#include "enemiesFunks.h"
#include "entityFunks.h"
#include "attackFunks.h"

#define STUN_DURATION 0.4f

void EnemyFriction(Enemy *enemy)
{
    enemy->velocity = Vector2Lerp(enemy->velocity, (Vector2){0, 0}, enemy->friction * GetFrameTime());
}

void EnemyMovement(Enemy *enemy, Vector2 target, GameData *gameData)
{
    // Calculate direction from enemy to target
    Vector2 direction = Vector2Subtract(target, enemy->position);
    enemy->direction = direction; // Update enemy direction
    // Calculate distance to target
    float distance = Vector2Length(direction);

    if (distance > 0)
    {
        // Normalize the direction vector
        direction = Vector2Normalize(direction);

        // accelerate in the direction of the target
        enemy->velocity = Vector2Add(enemy->velocity, Vector2Scale(direction, enemy->acceleration * GetFrameTime()));

        // Clamp velocity to the enemy's maximum speed
        if (Vector2Length(enemy->velocity) >= enemy->speed)
        {
            enemy->velocity = Vector2Scale(Vector2Normalize(enemy->velocity), enemy->speed);
        }
    }
    else {
        EnemyFriction(enemy);
    }
}

void EnemyAttack(Enemy *enemy, GameData *gameData)
{
    EnemyFriction(enemy);
    AttackUpdate(&enemy->attack, enemy->animationTime, enemy->position, enemy->direction, gameData);
    if (enemy->attack.done)
    {
        enemy->animationTime = 0.0f;
        enemy->state = EnemyStates::Neutral;
    }
}

void EnemyStartAttack(Enemy *enemy, GameData *gameData)
{
    switch (enemy->type)
    {
    case ENEMY_MELEE:
        enemy->attack = CreateAttack(enemy, AttackType::testEnemyMelee);
        break;

    case ENEMY_RANGED:
        enemy->attack = CreateAttack(enemy, AttackType::testEnemyRanged);
        break;
    case ENEMY_NONE: 
        // Why is this a thing that exists? - N
        break;
    }
    enemy->animationTime = 0.0f;
    enemy->state = EnemyStates::Attacking;
    EnemyAttack(enemy, gameData);
}

bool EnemyLineOfSight(Enemy *enemy, Player *player, Room *room)
{
    // Check if the enemy has line of sight to the player
    // This is a simple implementation, you might want to use raycasting or other methods for more complex scenarios
    bool canSee = false;
    for (int j = -rayCount; j <= rayCount / 2; j++)
    {
        // Calculate the angle offset for the ray based on the enemy's vision angle
        float angleOffset = ((float)j / (rayCount - 1) - 0.5f) * enemy->visionAngle * DEG2RAD;

        float angle = atan2f(enemy->direction.y, enemy->direction.x) + angleOffset;

        // Start the ray at the enemy's current position.
        Vector2 ray = enemy->position;
        for (int i = 0; i < enemy->visionRange; i++)
        {
            // take a step in the direction of the ray
            ray.x += cosf(angle) * tileSize;
            ray.y += sinf(angle) * tileSize;

            // check if the ray is within the vision range of the enemy
            if (ray.x < 0 || ray.x > GetScreenWidth() || ray.y < 0 || ray.y > GetScreenHeight())
            {
                break; // Ray is out of bounds
            }

            // Compute the corresponding grid cell by converting coordinates.
            int rayGridX = (int)(ray.x / tileSize);
            int rayGridY = (int)(ray.y / tileSize);

            if (!GetTile(room, rayGridX, rayGridY).isWalkable)
            {
                break; // Ray hit an obstacle.
            }
            // if the code has goten here the ray is within bounds and not blocked by an obstacle

            // Convert player's world position to the grid coordinate.
            int playerGridX = (int)(player->position.x / tileSize);
            int playerGridY = (int)(player->position.y / tileSize);

            if (rayGridX == playerGridX && rayGridY == playerGridY)
            {
                canSee = true;       // Player spotted!
                enemy->aware = true; // Set the enemy as aware of the player.
                break;               // Exit the inner loop since the player is found.
            }
        }
        if (canSee) // early exit checker
        {
            break; // Exit the outer loop since the player is found.
        }
    }

    return canSee; // Return true if the enemy can see the player, false otherwise.
}

void EnemyNeutral(Enemy *enemy, GameData *gameData)
{
    Player *player = &gameData->player;
    Room *room = gameData->currentRoom;

    // Move the enemy towards the target (player)
    // EnemyMovement target is the player position currently but will probably be changed in the future to allow for more advanced AI
    // TODO : Implement more advanced AI for enemy movement
    // - four types of enemy pathing, ranged (Tries to stay at a good range to hit the player), flanking (tries to cut off the player), guard (guards their target, for example a ranged unit), rush (Just charges at the player which is basically the current behavior but pathfinding is not implemented yet)

    // - enemy pathfinding (A* or Dijkstra's algorithm) to find the best path to the player
    Vector2 target = GetFlowFieldDirection((int)(enemy->position.x / tileSize), (int)(enemy->position.y / tileSize)); // Get the flow field direction for the enemy
    EnemyMovement(enemy, target, gameData);
    enemy->attackCooldownTimer += GetFrameTime(); // Update the attack cooldown timer

    // Check if the enemy is aware of the player
    if (enemy->aware == false)
    {
        EnemyLineOfSight(enemy, player, room); // Check if the enemy can see the player
    }
    if (enemy->aware == true) // two if statements on both values of aware is intended and not a mistake
    {
        float distance = Vector2Distance(enemy->position, player->position);
        if (distance <= enemy->attackRange)
        {
            EnemyStartAttack(enemy, gameData);
        }
    }
}

void EnemyUpdate(Enemy *enemy, GameData *gameData)
{

    enemy->animationTime += GetFrameTime();

    if (!enemy->alive)
    {
        return;
    }
    if (enemy->stunTimer > 0.0f)
    {
        enemy->stunTimer -= GetFrameTime(); // Update the stun timer
        return;
    }

    switch (enemy->state)
    {
    case EnemyStates::Neutral:
    {
        EnemyNeutral(enemy, gameData);
        break;
    }
    case EnemyStates::Attacking:
        EnemyAttack(enemy, gameData);
        break;
    }

    // Update the enemy's position using its current velocity
    Vector2 move = Vector2Scale(enemy->direction, GetFrameTime());
    EntityMove(&enemy->position, move, enemy->width, enemy->height, gameData);
}

Enemies CreateEnemies(EnemySeeder *seeder)
{
    Enemies enemies;
    enemies.count = seeder->count;                                    // Set the number of enemies
    enemies.enemies = (Enemy *)malloc(sizeof(Enemy) * enemies.count); // Allocate memory for the enemies array
    for (int i = 0; i < enemies.count; i++)
    {
        enemies.enemies[i].behavior = seeder->behavior[i];
        enemies.enemies[i].health = 100;
        enemies.enemies[i].damage = 15;
        enemies.enemies[i].visionRange = 25 * tileSize; // 5 tiles
        enemies.enemies[i].visionAngle = 90.0f;         // 90 degrees
        enemies.enemies[i].speed = 2.0f;
        enemies.enemies[i].position = seeder->positions[i]; // Set the position of the enemy
        enemies.enemies[i].alive = true;                    // Set the enemy as
        enemies.enemies[i].width = 16;
        enemies.enemies[i].height = 16;
        enemies.enemies[i].state = EnemyStates::Neutral;
        enemies.enemies[i].animationTime = 0.0f;
        enemies.enemies[i].friction = 15.0f;
        switch (seeder->type[i])
        {
        case ENEMY_MELEE:
            enemies.enemies[i].type = ENEMY_MELEE;
            enemies.enemies[i].attackRange = 0.5f * tileSize; // .5 tile
            enemies.enemies[i].attackDamage = 10;
            enemies.enemies[i].attackCooldown = 0.3f; // .3 second cooldown
            enemies.enemies[i].attackCooldownTimer = 0.0f;
            enemies.enemies[i].acceleration = 0.8f; // Acceleration of the enemy
            break;
        case ENEMY_RANGED:
            enemies.enemies[i].type = ENEMY_RANGED;
            enemies.enemies[i].attackRange = 10 * tileSize; // 10 tiles
            enemies.enemies[i].attackDamage = 8;
            enemies.enemies[i].attackCooldown = 0.8f; // .8 second cooldown
            enemies.enemies[i].attackCooldownTimer = 0.0f;
            enemies.enemies[i].acceleration = 1.0f; // Acceleration of the enemy
            break;
        default:
            break;
        }
    }

    return enemies;
}

void EnemyGetHit(Enemy *enemy, float damage, Vector2 force)
{
    enemy->health -= damage;
    enemy->velocity = Vector2Add(enemy->velocity, force);

    if (enemy->state == EnemyStates::Attacking)
    {
        AttackForceEnd(&enemy->attack);
    }

    if (enemy->health <= 0.0f)
    {
        enemy->alive = false;
        return;
    }

    enemy->stunTimer = STUN_DURATION;
}

void EnemyDraw(Enemy *enemy)
{
    switch (enemy->type)
    {
    case ENEMY_MELEE:
        DrawRectangle((int)(enemy->position.x - ((enemy->width + 1) >> 1)), (int)(enemy->position.y - ((enemy->height + 1) >> 1)), (int)enemy->width, (int)enemy->height, RED);
        break;

    case ENEMY_RANGED:
        DrawRectangle((int)(enemy->position.x - ((enemy->width + 1) >> 1)), (int)(enemy->position.y - ((enemy->height + 1) >> 1)), (int)enemy->width, (int)enemy->height, PURPLE);
        break;

    default:
        DrawText("ERROR", (int)enemy->position.x, (int)enemy->position.y, 20, RED);
        break;
    }

    if (!enemy->alive)
    {
        DrawCircle(enemy->position.x, enemy->position.x, enemy->width / 2, BLACK);
        return;
    }

    if (enemy->aware) {
        DrawCircle(enemy->position.x, enemy->position.x, enemy->width / 3, YELLOW);
    }

    switch (enemy->state)
    {
    case EnemyStates::Neutral:
        break;
    case EnemyStates::Attacking:
        DrawCircle(enemy->position.x, enemy->position.x, enemy->width / 2, BLUE);
        AttackDebugDraw(&enemy->attack, enemy->position, enemy->direction);
        break;
    }


    DrawLine(enemy->position.x, enemy->position.y, enemy->position.x + enemy->direction.x *10, enemy->position.y + enemy->direction.y * 10, WHITE);
}