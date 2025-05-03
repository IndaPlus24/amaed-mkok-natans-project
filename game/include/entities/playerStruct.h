#ifndef PLAYER_S_H
#define PLAYER_S_H

#include <raylib.h>
#include <raymath.h>

enum class PlayerState {
    Neutral,
    Attack,
};

typedef struct Player
{
    int health;
    // int damage;
    PlayerState state;
    float animationTime;
    float speed;            // Max speed
    float acceleration;     // Acceleration rate
    Vector2 velocity;       // Current velocity
    Vector2 position;       // Current position
    Vector2 direction;      // Current facing direction
    int width;              // Collision width
    int height;             // Collision height

} Player;


#endif