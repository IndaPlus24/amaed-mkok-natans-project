#ifndef ATTACK_S_H
#define ATTACK_S_H

#include <unordered_set>

#include <raylib.h>

#include "collisionLayers.h"

enum class AttackType {
    testMelee,
    testRanged,
    testEnemyMelee,
    testEnemyRanged,
};

enum class AttackAction
{
    ActivateHitBox,
    DeactivateHitBox,
    SpawnProjectile,
    SetFrame,
    ResetHits,
    DoSomeReallyWackyShit, // You want some one-off hard-coded shit? This is how you do it.
    End,
};


typedef struct HitBox
{
    bool active;
    int width;
    int height;
    Vector2 offset; // Rotates with the owner's direction, positive x is "forwards"
} HitBox;

typedef struct KeyFrame
{
    float time;
    AttackAction action;
    unsigned long long data;
} KeyFrame;

typedef struct Attack
{
    void *owner; // Make sure we don't hit the owner (unless we want to)
    std::unordered_set<void *> hits;  // Make sure we don't hit the same target multiple times (unless we want to)

    CollisionLayers targets;

    float damage;
    float force;

    // float lastCheck; // Timestamp of the last time this thing was checked

    int currentFrame; // The frame of the animation to display

    KeyFrame *keyFrames;
    int keyFrameCount;
    int currentKeyIndex;

    HitBox *hitBoxes;
    int hitBoxCount;

    bool done;
} Attack;

#endif