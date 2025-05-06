#ifndef COLLISION_H
#define COLLISION_H

enum class CollisionLayers {
    Player      = 0b00000001,
    Enemies     = 0b00000010,
    Corpses     = 0b00000100, // Don't judge: maybe someone is into that
    Self        = 0b00001000, // Don't judge: maybe someone is into that
    Walls       = 0b00010000, // Don't judge: maybe someone is into that
    Projectiles = 0b00100000, // Don't judge: maybe someone is into that
};
#endif