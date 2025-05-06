#ifndef PLAYER_S_H
#define PLAYER_S_H

#include <raylib.h>
#include <raymath.h>

#include "spriteSheet.h"
#include "attackStruct.h"

#define PLAYER_SPRITE_SHEETS 1

enum class PlayerState {
    Neutral,
    Attack,
    Stunned,
    Dead,
};

typedef struct Player
{
    float health;
    PlayerState state;
    float animationTime;
    float speed;            // Max speed
    float acceleration;     // Acceleration rate
    float friction;         // Friction thingy
    Vector2 velocity;       // Current velocity
    Vector2 position;       // Current position
    Vector2 direction;      // Current facing direction
    int width;              // Collision width
    int height;             // Collision height
    Attack attack;
    float invincibilityTimer;
    float invincibilityDuration;
    SpriteSheet sheets[PLAYER_SPRITE_SHEETS];
} Player;


#endif