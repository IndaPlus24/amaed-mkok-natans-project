#include <raylib.h>
#include <raymath.h>

#include "attackFunks.h"

Attack CreateAttack(void *owner, AttackType attackType)
{
    Attack attack;
    attack.currentFrame = 0;
    // attack.lastCheck = 0;
    attack.currentFrame = 0;
    attack.currentKeyIndex = 0;
    attack.done = false;
    attack.owner = owner;
    attack.hitsCount = 0;
    attack.hits = (void **)malloc(sizeof(void **) * 100);
    switch (attackType)
    {
    case AttackType::testMelee:
        attack.targets = Targets::Enemies;

        attack.keyFrameCount = 5;
        attack.keyFrames = (KeyFrame *)malloc(sizeof(KeyFrame) * attack.keyFrameCount);

        // 0.0 - 0.01: buildup

        // 0.01 - 0.3: active
        attack.keyFrames[0].time = 0.01;
        attack.keyFrames[0].action = AttackAction::ActivateHitBox;
        attack.keyFrames[0].data = 0;

        attack.keyFrames[1].time = 0.01;
        attack.keyFrames[1].action = AttackAction::SetFrame;
        attack.keyFrames[1].data = 1;

        // 0.3 - 0.5: recovery
        attack.keyFrames[2].time = 0.3;
        attack.keyFrames[2].action = AttackAction::DeactivateHitBox;
        attack.keyFrames[2].data = 0;

        attack.keyFrames[3].time = 0.3;
        attack.keyFrames[3].action = AttackAction::SetFrame;
        attack.keyFrames[3].data = 2;

        // End of animation
        attack.keyFrames[4].time = 0.5;
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

    default:
        break;
    }
    return attack;
}

bool AddHit(Attack *attack, void *pointer)
{
    for (int i = 0; i < attack->hitsCount; i++)
    {
        if (attack->hits[i] == pointer)
        {
            return false;
        }
    }
    attack->hits[attack->hitsCount] = pointer;
    attack->hitsCount++;
    return true;
}

void ThingThatEvaluatesHitBoxes(const HitBox *hitBox, Attack *attack, Vector2 pos, Vector2 dir, GameData *gameData)
{
    Vector2 offset;
    offset.x = dir.x * hitBox->offset.x - dir.y * hitBox->offset.y;
    offset.y = dir.y * hitBox->offset.x + dir.x * hitBox->offset.y;

    offset = Vector2Add(offset, pos);
    float x0 = offset.x - hitBox->width / 2;
    float y0 = offset.y - hitBox->height / 2;

    float x1 = x0 + hitBox->width;
    float y1 = y0 + hitBox->height;

    if ((int)attack->targets & (int)Targets::Enemies)
    {
        for (int i = 0; i < gameData->enemies.count; i++)
        {
            Enemy *e = &gameData->enemies.enemies[i];
            // If it is dead and we aren't looking for dead ones, then we skip it
            if ((!((int)attack->targets & (int)Targets::Corpses) && !e->alive) || ((int)attack->targets & (int)Targets::Self && attack->owner == e))
            {
                continue;
            }

            float ex0 = e->position.x - e->width / 2;
            float ex1 = ex0 + e->width;
            float ey0 = e->position.y - e->height / 2;
            float ey1 = ey0 + e->height;

            // The standard overlapping square check
            if (ex1 > x0 && ex0 < x1 && ey1 > y0 && ey0 < y1)
            {
                if (AddHit(attack, e))
                {
                    // hit
                }
            }
        }
    }

    // Please, bear with me

    // It targets a player
    if ((int)attack->targets & (int)Targets::Player)
    {
        // Either it is able to target self of the target isn't self
        if ((((int)attack->targets & (int)Targets::Self || attack->owner != &gameData->player)))
        {
            // It either targets corpses or the target isn't a corpse
            if (((int)attack->targets & (int)Targets::Corpses && gameData->player.state != PlayerState::Dead))
            {
                float px0 = gameData->player.position.x - gameData->player.width / 2;
                float px1 = px0 + gameData->player.width;
                float py0 = gameData->player.position.y - gameData->player.height / 2;
                float py1 = py0 + gameData->player.height;
                // The standard overlapping square check
                if (px1 > x0 && px0 < x1 && py1 > y0 && py0 < y1)
                {
                    if (AddHit(attack, &gameData->player))
                    {
                        // hit
                    }
                }
            }
        }
    }
    // I think I blacked out.
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
        // TODO: Get back and fix this when I actually have projectiles
        break;

    case AttackAction::ResetHits:
        attack->hitsCount = 0;
        break;

    case AttackAction::SetFrame:
        attack->currentFrame = keyFrame->data;
        break;

    case AttackAction::DoSomeReallyWackyShit:
        // Shove keyFrame->data into the function that evaluates the wacky hard coded things.
        break;

    case AttackAction::End:
        free(attack->keyFrames);
        free(attack->hitBoxes);
        free(attack->hits);
        attack->done = true;
        break;
    }
}

void AttackUpdate(Attack *attack, float timestamp, Vector2 pos, Vector2 dir, GameData *gameData)
{
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
    if (attack->currentKeyIndex == attack->keyFrameCount && !attack->done)
    {
        free(attack->keyFrames);
        free(attack->hitBoxes);
        free(attack->hits);
        attack->done = true;
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
