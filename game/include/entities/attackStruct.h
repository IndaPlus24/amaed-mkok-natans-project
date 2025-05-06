#ifndef ATTACK_S_H
#define ATTACK_S_H

#include <raylib.h>

enum class Targets {
    Player      = 0b0001,
    Enemies     = 0b0010,
    Corpses     = 0b0100, // Don't judge: maybe someone is into that
    Self        = 0b1000, // Don't judge: maybe someone is into that
};

enum class AttackType {
    testMelee,
    testRanged,
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
    void **hits;  // Make sure we don't hit the same target multiple times (unless we want to)
    int hitsCount;

    Targets targets;

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