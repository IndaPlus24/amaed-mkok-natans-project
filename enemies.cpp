#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#define tileSize 32           // Size of the tile in pixels
#define rayCount 16           // Number of rays to cast for line of sight
#define DEG2RAD = 0.017453292 // Conversion factor from degrees to radians

typedef enum EnemyType
{
    NULL,
    MELEE,
    RANGED,
} EnemyType;

typedef enum EnemyBehavior
{
    NULL,
    GUARD,
    FLANK,
    RUSH,
    RANGED,
} EnemyBehavior;

typedef struct Enemy
{
    EnemyType type;         // Type of enemy (melee, ranged, etc.)
    EnemyBehavior behavior; // Behavior of the enemy (guard, flank, rush, ranged)
    bool aware;             // Is the enemy aware of the player?
    int health;
    int damage;
    int visionRange;           // Range of vision
    float visionAngle;         // Angle of vision (in degrees)
    int attackRange;           // Range of attack
    int attackDamage;          // Damage dealt by the enemy
    float attackCooldown;      // Cooldown time between attacks (measured in seconds)
    float attackCooldownTimer; // Timer for attack cooldown (time since last attack, if greater than attackCooldown, the enemy can attack again)
    float speed;
    float acceleration; // Acceleration of the enemy
    Vector2 velocity;   // Velocity of the enemy
    Vector2 position;
    Vector2 direction;
    bool alive;
} Enemy;
typedef struct Enemies
{
    Enemy *enemies; // Array of enemies
    int count;      // Number of enemies
} Enemies;

typedef struct EnemySeeder
{
    Vector2[] positions;      // Array of enemy positions
    int count;                // Number of enemies
    EnemyType[] type;         // Array of type of enemy (melee, ranged, etc.)
    EnemyBehavior[] behavior; // Array of behavior of the enemy (guard, flank, rush, ranged)
} EnemySeeder;

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
    if (distance <= enemy->attackRange)
    {
        // Attack logic here
        player->health -= enemy->damage;
        enemy->attackCooldown = 1.0f; // Reset cooldown after attack
    }
}
bool EnemyLineOfSight(Enemy *enemy, Player *player, Tile *tileMap)
{
    // Check if the enemy has line of sight to the player
    // This is a simple implementation, you might want to use raycasting or other methods for more complex scenarios
    float distance = Vector2Distance(enemy->position, player->position);
    bool canSee = false;
    Vector2 ray = enemy->position;
    for (int j = -rayCount; j < rayCount / 2; j++)
    {
        // Calculate the angle offset for the ray based on the enemy's vision angle
        float angleOffset = ((float)j / (rayCount - 1) - 0.5f) * enemy->visionAngle * DEG2RAD;

        float angle = atan2f(enemy->direction.y, enemy->direction.x) + angleOffset;

        // Start the ray at the enemy's current position.
        Vector2 ray = enemy->position;
        for (int i = 0; i < enemy->visionRange; i++)
        {
            // take a step in the direction of the ray
            ray.x += cosf(angle) * i * tileSize;
            ray.y += sinf(angle) * i * tileSize;
            // check if the ray is within the vision range of the enemy
            if (ray.x < 0 || ray.x > GetScreenWidth() || ray.y < 0 || ray.y > GetScreenHeight())
            {
                break; // Ray is out of bounds
            }

            // Compute the corresponding grid cell by converting coordinates.
            int rayGridX = (int)(ray.x / tileSize);
            int rayGridY = (int)(ray.y / tileSize);

            if (!tileMap[gridX][gridY].walkable)
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

        EnemyMovement(enemy, player.position);

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
        enemies.enemies[i].visionAngle = 90.0f;        // 90 degrees
        enemies.enemies[i].speed = 2.0f;
        enemies.enemies[i].position = seeder->positions[i]; // Set the position of the enemy
        enemies.enemies[i].alive = true;                    // Set the enemy as alive
        switch (seeder->type[i])
        {
        case MELEE:
            enemies.enemies[i].type = MELEE;
            enemies.enemies[i].attackRange = 0.5f * tileSize; // 1 tile
            enemies.enemies[i].attackDamage = 10;
            enemies.enemies[i].attackCooldown = 0.3f; // .3 second cooldown
            enemies.enemies[i].attackCooldownTimer = 0.0f;
            enemies.enemies[i].acceleration = 0.8f;             // Acceleration of the enemy
            break;
        case RANGED:
            enemies.enemies[i].type = MELEE;
            enemies.enemies[i].attackRange = 10 * tileSize; // 1 tile
            enemies.enemies[i].attackDamage = 8;
            enemies.enemies[i].attackCooldown = 0.8f; // .8 second cooldown
            enemies.enemies[i].attackCooldownTimer = 0.0f;
            enemies.enemies[i].acceleration = 1.0f;             // Acceleration of the enemy
            break;
        default:
            break;
        }
    }

    return enemies;
}
