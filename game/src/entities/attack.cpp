#include <raylib.h>
#include <raymath.h>

#include "attackFunks.h"
#include "entityFunks.h"
#include "projectilesFunks.h"

Attack CreateAttack(void *owner, AttackType attackType)
{
    Attack attack;
    attack.currentFrame = 0;
    attack.currentFrame = 0;
    attack.currentKeyIndex = 0;
    attack.done = false;
    attack.owner = owner;
    attack.hits = new std::unordered_set<void *>();

    switch (attackType)
    {
    case AttackType::testMelee:
        attack.damage = 50.0f;
        attack.force = 300.0f;
        attack.targets = CollisionLayers::Enemies;

        attack.keyFrameCount = 5;
        attack.keyFrames = (KeyFrame *)malloc(sizeof(KeyFrame) * attack.keyFrameCount);

        // 0.0 - 0.01: buildup

        // 0.01 - 0.3: active
        attack.keyFrames[0].time = 0.01f;
        attack.keyFrames[0].action = AttackAction::ActivateHitBox;
        attack.keyFrames[0].data = 0;

        attack.keyFrames[1].time = 0.01f;
        attack.keyFrames[1].action = AttackAction::SetFrame;
        attack.keyFrames[1].data = 1;

        // 0.3 - 0.5: recovery
        attack.keyFrames[2].time = 0.3f;
        attack.keyFrames[2].action = AttackAction::DeactivateHitBox;
        attack.keyFrames[2].data = 0;

        attack.keyFrames[3].time = 0.3f;
        attack.keyFrames[3].action = AttackAction::SetFrame;
        attack.keyFrames[3].data = 2;

        // End of animation
        attack.keyFrames[4].time = 0.5f;
        attack.keyFrames[4].action = AttackAction::End;
        attack.keyFrames[4].data = 0;

        // Set up the HitBox
        attack.hitBoxCount = 1;
        attack.hitBoxes = (HitBox *)malloc(sizeof(HitBox) * attack.hitBoxCount);
        attack.hitBoxes[0].active = false;
        attack.hitBoxes[0].width = 20;
        attack.hitBoxes[0].height = 20;
        attack.hitBoxes[0].offset = Vector2{20, 0};
        break;

    case AttackType::testRanged:
        attack.damage = 50.0f;
        attack.force = 100.0f;
        attack.targets = CollisionLayers::Enemies;

        attack.keyFrameCount = 2;
        attack.keyFrames = (KeyFrame *)malloc(sizeof(KeyFrame) * attack.keyFrameCount);

        // Just fucking blast 'em. - N
        attack.keyFrames[0].time = 0.0f;
        attack.keyFrames[0].action = AttackAction::SpawnProjectile;
        attack.keyFrames[0].data = (int)ProjectilePrefabs::Default;

        attack.keyFrames[1].time = 0.0f;
        attack.keyFrames[1].action = AttackAction::End;
        attack.keyFrames[1].data = 0;

        attack.hitBoxCount = 0;
        attack.hitBoxes = nullptr; // (HitBox *)malloc(sizeof(HitBox) * attack.hitBoxCount);
        break;

    case AttackType::testEnemyMelee:
        // Mostly same as the normal test
        attack.damage = 20.0f;
        attack.force = 400.0f;
        attack.targets = CollisionLayers::Player;

        attack.keyFrameCount = 5;
        attack.keyFrames = (KeyFrame *)malloc(sizeof(KeyFrame) * attack.keyFrameCount);

        // 1.0- 1.0: buildup

        // 1.0 - 1.3: active
        attack.keyFrames[0].time = 1.0f;
        attack.keyFrames[0].action = AttackAction::ActivateHitBox;
        attack.keyFrames[0].data = 0;

        attack.keyFrames[1].time = 1.0f;
        attack.keyFrames[1].action = AttackAction::SetFrame;
        attack.keyFrames[1].data = 1;

        // 1.3 - 1.5: recovery
        attack.keyFrames[2].time = 1.3f;
        attack.keyFrames[2].action = AttackAction::DeactivateHitBox;
        attack.keyFrames[2].data = 0;

        attack.keyFrames[3].time = 1.3f;
        attack.keyFrames[3].action = AttackAction::SetFrame;
        attack.keyFrames[3].data = 2;

        // End of animation
        attack.keyFrames[4].time = 1.5f;
        attack.keyFrames[4].action = AttackAction::End;
        attack.keyFrames[4].data = 0;

        // Set up the HitBox
        attack.hitBoxCount = 1;
        attack.hitBoxes = (HitBox *)malloc(sizeof(HitBox) * attack.hitBoxCount);
        attack.hitBoxes[0].active = false;
        attack.hitBoxes[0].width = 20;
        attack.hitBoxes[0].height = 20;
        attack.hitBoxes[0].offset = Vector2{20, 0};
        break;

    case AttackType::testEnemyRanged:
        attack.damage = 50.0f;
        attack.force = 100.0f;
        attack.targets = CollisionLayers::Player;

        attack.keyFrameCount = 2;
        attack.keyFrames = (KeyFrame *)malloc(sizeof(KeyFrame) * attack.keyFrameCount);

        attack.keyFrames[0].time = 1.0f;
        attack.keyFrames[0].action = AttackAction::SpawnProjectile;
        attack.keyFrames[0].data = (int)ProjectilePrefabs::Default;

        attack.keyFrames[1].time = 1.2f;
        attack.keyFrames[1].action = AttackAction::End;
        attack.keyFrames[1].data = 0;

        attack.hitBoxCount = 0;
        attack.hitBoxes = nullptr; // (HitBox *)malloc(sizeof(HitBox) * attack.hitBoxCount);
        break;
    }
    return attack;
}

void ThingThatEvaluatesHitBoxes(const HitBox *hitBox, Attack *attack, Vector2 pos, Vector2 dir, GameData *gameData)
{
    Vector2 offset;
    offset.x = dir.x * hitBox->offset.x - dir.y * hitBox->offset.y;
    offset.y = dir.y * hitBox->offset.x + dir.x * hitBox->offset.y;

    offset = Vector2Add(offset, pos);

    std::unordered_set<void *> newHits = GetEntitiesInArea(offset, hitBox->width, hitBox->height, attack->targets, gameData, attack->owner);

    for (std::unordered_set<void *>::iterator itr = newHits.begin(); itr != newHits.end(); ++itr)
    {
        if (attack->hits->insert(*itr).second)
        {
            // It Hit!
            AttackEntity(*itr, attack->owner, gameData, attack->damage, Vector2Scale(dir, attack->force));
        }
    }
}

void ThingThatEvaluatesKeyFrames(const KeyFrame *keyFrame, Attack *attack, Vector2 pos, Vector2 dir, GameData *gameData)
{
    switch (keyFrame->action)
    {
    case AttackAction::ActivateHitBox:
        attack->hitBoxes[keyFrame->data].active = true;
        ThingThatEvaluatesHitBoxes(&attack->hitBoxes[keyFrame->data], attack, pos, dir, gameData);
        break;

    case AttackAction::DeactivateHitBox:
        attack->hitBoxes[keyFrame->data].active = false;
        break;

    case AttackAction::SpawnProjectile:
        CreateProjectile((ProjectilePrefabs)keyFrame->data, gameData, attack->owner, dir, attack->damage, attack->force);
        break;

    case AttackAction::ResetHits:
        attack->hits->clear();
        break;

    case AttackAction::SetFrame:
        attack->currentFrame = keyFrame->data;
        break;

    case AttackAction::DoSomeReallyWackyShit:
        // Shove keyFrame->data into the function that evaluates the wacky hard coded things.
        break;

    case AttackAction::End:
        AttackForceEnd(attack);
        break;
    }
}

void AttackUpdate(Attack *attack, float timestamp, Vector2 pos, Vector2 dir, GameData *gameData)
{
    if (attack->done)
        return;

    for (int i = 0; i < attack->hitBoxCount; i++)
    {
        if (attack->hitBoxes[i].active)
        {
            // Shove attack->hitBoxes[i] into the function that evaluates HitBoxes
            ThingThatEvaluatesHitBoxes(&attack->hitBoxes[i], attack, pos, dir, gameData);
        }
    }

    for (; attack->currentKeyIndex < attack->keyFrameCount; attack->currentKeyIndex++)
    {
        if (attack->keyFrames[attack->currentKeyIndex].time < timestamp)
        {
            // Shove attack->keyFrames[attack->currentKeyIndex] into the thing that evaluates keyframes
            ThingThatEvaluatesKeyFrames(&attack->keyFrames[attack->currentKeyIndex], attack, pos, dir, gameData);
        }
        else
        {
            break;
        }
    }

    // They are supposed to have an AttackAction::End at the end, but I don't want memory leaks because people are stupid
    if (attack->currentKeyIndex == attack->keyFrameCount)
    {
        AttackForceEnd(attack);
    }
}

void AttackDebugDraw(const Attack *attack, Vector2 pos, Vector2 dir)
{
    for (int i = 0; i < attack->hitBoxCount; i++)
    {
        HitBox *hitBox = &attack->hitBoxes[i];
        if (attack->hitBoxes[i].active)
        {
            Vector2 offset;
            offset.x = dir.x * hitBox->offset.x - dir.y * hitBox->offset.y;
            offset.y = dir.y * hitBox->offset.x + dir.x * hitBox->offset.y;

            offset = Vector2Add(offset, pos);
            float x0 = offset.x - hitBox->width / 2;
            float y0 = offset.y - hitBox->height / 2;

            DrawRectangle(x0, y0, hitBox->width, hitBox->height, DARKGREEN);
        }
    }
}

void AttackForceEnd(Attack *attack)
{
    if (attack->done)
        return;

    free(attack->keyFrames);
    if (attack->hitBoxCount)
    {
        free(attack->hitBoxes);
    }
    attack->hits->~unordered_set();
    attack->done = true;
}
