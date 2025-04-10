#ifndef ENEMIES_H
#define ENEMIES_H

#include <stdbool.h>
#include <raylib.h>
#include <raymath.h>
#include "AI.h"  

// -- Macro Definitions --
#define tileSize 32         // Size of a tile in pixels
#define rayCount 16         // Number of rays to cast for line of sight
#define DEG2RAD 0.017453292 // Conversion factor from degrees to radians

// -- Forward Declarations --
// These should be defined elsewhere.
typedef struct Player Player;
typedef struct Tile Tile;

// -- Type Definitions --

// Enemy types.
typedef enum EnemyType
{
    ENEMY_NONE,
    ENEMY_MELEE,
    ENEMY_RANGED,
} EnemyType;

// Enemy behaviors.
typedef enum EnemyBehavior
{
    BEHAVIOR_NONE,
    BEHAVIOR_GUARD,
    BEHAVIOR_FLANK,
    BEHAVIOR_RUSH,
    BEHAVIOR_RANGED,
} EnemyBehavior;

// Single enemy definition.
typedef struct Enemy
{
    EnemyType type;         // Type of enemy (melee, ranged, etc.)
    EnemyBehavior behavior; // Behavior of the enemy (guard, flank, rush, ranged)
    bool aware;             // Is the enemy aware of the player?
    int health;
    int damage;
    int visionRange;        // Range of vision (in pixels)
    float visionAngle;      // Angle of vision (in degrees)
    int attackRange;        // Range of attack (in pixels)
    int attackDamage;       // Damage dealt by the enemy
    float attackCooldown;   // Cooldown time between attacks (in seconds)
    float attackCooldownTimer; // Timer for attack cooldown
    float speed;
    float acceleration;     // Acceleration of the enemy
    Vector2 velocity;       // Current velocity
    Vector2 position;       // Current position
    Vector2 direction;      // Current facing direction
    bool alive;
} Enemy;

// Container for an array of enemies.
typedef struct Enemies
{
    Enemy *enemies; // Array of enemies
    int count;      // Number of enemies in the array
} Enemies;

// Structure for seeding enemies.
typedef struct EnemySeeder
{
    Vector2* positions;      // Array of enemy starting positions
    int count;               // Number of enemies to seed
    EnemyType* type;         // Array of enemy types
    EnemyBehavior* behavior; // Array of enemy behaviors
} EnemySeeder;

// -- Function Prototypes --
void EnemyMovement(Enemy *enemy, Vector2 target);
void EnemyAttack(Enemy *enemy, Player *player);
bool EnemyLineOfSight(Enemy *enemy, Player *player, Tile *tileMap);
void EnemyUpdate(Enemy *enemy, Player *player, Tile *tileMap);
Enemies CreateEnemies(EnemySeeder *seeder);

#endif // ENEMIES_H
