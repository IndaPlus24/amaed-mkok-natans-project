#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>
#include "enemies.h"



void EnemyMovement(Enemy *enemy, Vector2 target)
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

        // Update the enemy's position using its current velocity
        enemy->position = Vector2Add(enemy->position, Vector2Scale(enemy->velocity, GetFrameTime()));
    }
}
void EnemyAttack(Enemy *enemy, Player *player)
{
    // Check if the enemy is within attack range of the target
    float distance = Vector2Distance(enemy->position, player->position);
    if (distance <= enemy->attackRange && enemy->attackCooldownTimer >= enemy->attackCooldown)
    {
        // Attack logic here
        player->health -= enemy->damage;
        enemy->attackCooldownTimer = 0.0f; // Reset cooldown after attack
    }
}
bool EnemyLineOfSight(Enemy *enemy, Player *player, Tile *tileMap)
{
    // Check if the enemy has line of sight to the player
    // This is a simple implementation, you might want to use raycasting or other methods for more complex scenarios
    float distance = Vector2Distance(enemy->position, player->position);
    bool canSee = false;
    Vector2 ray = enemy->position;
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

            if (!tileMap[rayGridX][rayGridY].walkable)
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

void EnemyUpdate(Enemy *enemy, Player *player, Tile *tileMap)
{
    if (enemy->alive)
    {
        // Move the enemy towards the target (player)
        // EnemyMovement target is the player position currently but will probably be changed in the future to allow for more advanced AI
        // TODO : Implement more advanced AI for enemy movement
        // - four types of enemy pathing, ranged (Tries to stay at a good range to hit the player), flanking (tries to cut off the player), guard (guards their target, for example a ranged unit), rush (Just charges at the player which is basically the current behavior but pathfinding is not implemented yet)

        // - enemy pathfinding (A* or Dijkstra's algorithm) to find the best path to the player
        Vector2 target = GetFlowFieldDirection((int)(enemy->position.x / tileSize), (int)(enemy->position.y / tileSize)); // Get the flow field direction for the enemy
        EnemyMovement(enemy, target);
        enemy->attackCooldownTimer += GetFrameTime(); // Update the attack cooldown timer

        // Check if the enemy is aware of the player
        if (enemy->aware == false)
        {
            EnemyLineOfSight(enemy, player, tileMap); // Check if the enemy can see the player
        }
        if (enemy->aware == true) // two if statments on both values of aware is intended and not a mistake
        {
            float distance = Vector2Distance(enemy->position, player->position);
            if (distance <= enemy->attackRange)
            {
                EnemyAttack(enemy, player);
            }
        }
    }
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
        enemies.enemies[i].alive = true;                    // Set the enemy as alive
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
