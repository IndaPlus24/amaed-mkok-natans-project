#include "projectilesFunks.h"
#include "entityFunks.h"
#include "attackFunks.h"

#ifndef reallocarray
    #include <cstdlib>  // For realloc
    #define reallocarray(ptr, count, size) realloc(ptr, (count) * (size))
#endif

Projectile *ProjectilesPush(GameData *gameData)
{
    if (gameData->projectiles.count == gameData->projectiles.capacity)
    {
        gameData->projectiles.capacity *= 2;
        gameData->projectiles.list = (Projectile *)reallocarray(gameData->projectiles.list, gameData->projectiles.capacity, sizeof(Projectile));
    }

    Projectile *ptr = &gameData->projectiles.list[gameData->projectiles.count];

    gameData->projectiles.count++;
    return ptr;
}

void ThingThatEvaluatesProjectileHitBoxes(const HitBox *hitBox, Projectile *projectile, GameData *gameData)
{
    Vector2 offset;
    offset.x = projectile->direction.x * hitBox->offset.x - projectile->direction.y * hitBox->offset.y;
    offset.y = projectile->direction.y * hitBox->offset.x + projectile->direction.x * hitBox->offset.y;

    offset = Vector2Add(offset, projectile->position);

    std::unordered_set<void *> newHits = GetEntitiesInArea(offset, hitBox->width, hitBox->height, projectile->targets, gameData, projectile->owner);

    for (std::unordered_set<void *>::iterator itr = newHits.begin(); itr != newHits.end(); ++itr)
    {
        if (projectile->hits->insert(*itr).second)
        {
            // It Hit!
            AttackEntity(*itr, projectile->owner, gameData, projectile->damage, Vector2Scale(projectile->direction, projectile->force));

            projectile->pierces--;
        }
    }
}

Projectile *CreateProjectile(ProjectilePrefabs prefab, GameData *gameData, void *owner, Vector2 direction, float damage, float force)
{
    Projectile *ptr = ProjectilesPush(gameData);

    ptr->owner = owner;
    ptr->position = GetEntityPosition(owner, gameData);
    ptr->direction = direction;

    ptr->damage = damage;
    ptr->force = force;

    ptr->hits = new std::unordered_set<void *>();

    switch (prefab)
    {
    case ProjectilePrefabs::Default:
    {
        ptr->behaviour = ProjectileBehaviour::Straight;
        ptr->speed = 500.0f;

        ptr->targets = (CollisionLayers)(((int)CollisionLayers::Player | (int)CollisionLayers::Enemies) ^ (int)GetEntityType(owner, gameData));
        ptr->pierces = 0;
        ptr->bounces = 0;

        ptr->hitBoxCount = 1;
        ptr->hitBoxes = (HitBox *)malloc(sizeof(HitBox) * ptr->hitBoxCount);

        ptr->hitBoxes[0].active = true;
        ptr->hitBoxes[0].width = 10;
        ptr->hitBoxes[0].height = 10;
        ptr->hitBoxes[0].offset = Vector2Zero();
        break;
    }
    case ProjectilePrefabs::Slow:
    {
        ptr->behaviour = ProjectileBehaviour::Straight;
        ptr->speed = 500.0f;

        ptr->targets = (CollisionLayers)(((int)CollisionLayers::Player | (int)CollisionLayers::Enemies) ^ (int)GetEntityType(owner, gameData));
        ptr->pierces = 0;
        ptr->bounces = 0;

        ptr->hitBoxCount = 1;
        ptr->hitBoxes = (HitBox *)malloc(sizeof(HitBox) * ptr->hitBoxCount);

        ptr->hitBoxes[0].active = true;
        ptr->hitBoxes[0].width = 10;
        ptr->hitBoxes[0].height = 10;
        ptr->hitBoxes[0].offset = Vector2Zero();
        break;
    }
    }

    return ptr;
}

void ProjectileUpdate(Projectile *projectile, GameData *gameData)
{
    switch (projectile->behaviour)
    {
    case ProjectileBehaviour::Straight:
    {
        Vector2 move = Vector2Scale(projectile->direction, projectile->speed * GetFrameTime());
        bool walled = EntityMove(&projectile->position, move, projectile->hitBoxes[0].width, projectile->hitBoxes[0].height, gameData);

        for (int i = 0; i < projectile->hitBoxCount; i++)
        {
            ThingThatEvaluatesProjectileHitBoxes(projectile->hitBoxes + i, projectile, gameData);
        }

        if ((walled && !projectile->bounces) || projectile->pierces < 0)
        {
            DestroyProjectile(projectile, gameData);
        }
        else if (walled && projectile->bounces)
        {
            projectile->bounces--;
            // Make it bounce somehow - N
        }
        break;
    }
    }
}

void DestroyProjectile(Projectile *projectile, GameData *gameData)
{
    free(projectile->hitBoxes);
    projectile->hits->~unordered_set();
    // Hopefully it will realize that things are going out of scope so it calls the deconstructor or whatever.
    // I don't really know how that works if it wasn't obvious.
    *projectile = gameData->projectiles.list[gameData->projectiles.count - 1];

    gameData->projectiles.count--;
}

void ProjectileDebugDraw(const Projectile *projectile)
{
    for (int i = 0; i < projectile->hitBoxCount; i++)
    {
        HitBox *hitBox = &projectile->hitBoxes[i];
        if (projectile->hitBoxes[i].active)
        {
            Vector2 offset;
            offset.x = projectile->direction.x * hitBox->offset.x - projectile->direction.y * hitBox->offset.y;
            offset.y = projectile->direction.y * hitBox->offset.x + projectile->direction.x * hitBox->offset.y;

            offset = Vector2Add(offset, projectile->position);
            float x0 = offset.x - hitBox->width / 2;
            float y0 = offset.y - hitBox->height / 2;

            DrawRectangle(x0, y0, hitBox->width, hitBox->height, DARKGREEN);
        }
    }
}
