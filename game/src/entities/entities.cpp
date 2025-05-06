#include <unordered_set>

#include "entityFunks.h"
#include "playerFunks.h"

bool EntityMove(Vector2 *pos, Vector2 move, int width, int height, GameData *gameData)
{
    bool stopX = false;

    // Horizontal
    if (move.x != 0)
    {
        int sign = move.x > 0 ? 1 : -1;
        float margin = floorf(sign * ((float)width) / 2);

        int y0 = (int)((pos->y - height / 2) / tileSize);      // y start
        int y1 = (int)((pos->y + height / 2 - 1) / tileSize);  // y end
        int x0 = (int)((pos->x) / tileSize);                   // x start
        int x1 = (int)((pos->x + move.x + margin) / tileSize); // x limit

        // Did it hit a wall?

        // loop from x0 to x1 (inclusive)
        for (int x = x0; x != x1 + sign; x += sign)
        {
            for (int y = y0; y <= y1; y++)
            {
                if (!GetTile(gameData->currentRoom, x, y).isWalkable)
                {
                    stopX = true;
                    break;
                }
            }
            if (stopX)
            {
                int lastTile = x - sign; // The x of the tile they would have ended up in

                pos->x = (lastTile + (sign + 1) / 2) * tileSize - margin;
                break;
            }
        }

        if (!stopX)
        {
            pos->x += move.x;
        }
    }

    bool stopY = false;
    // Vertical
    if (move.y != 0)
    {
        int sign = move.y > 0 ? 1 : -1;
        float margin = floorf(sign * ((float)height) / 2);

        int y0 = (int)((pos->y) / tileSize);                   // y start
        int y1 = (int)((pos->y + move.y + margin) / tileSize); // y limit
        int x0 = (int)((pos->x - width / 2) / tileSize);       // x start
        int x1 = (int)((pos->x + width / 2 - 1) / tileSize);   // x end

        // Did it hit a wall?

        // loop from y0 to y1 (inclusive)
        for (int y = y0; y != y1 + sign; y += sign)
        {
            for (int x = x0; x <= x1; x++)
            {
                if (!GetTile(gameData->currentRoom, x, y).isWalkable)
                {
                    stopY = true;
                }
            }
            if (stopY)
            {
                int lastTile = y - sign; // The y of the tile they would have ended up in

                pos->y = (lastTile + (sign + 1) / 2) * tileSize - margin;
                break;
            }
        }

        if (!stopY)
        {
            pos->y += move.y;
        }
    }

    return stopX || stopY;
}

std::unordered_set<void *> GetEntitiesInArea(Vector2 pos, int width, int height, CollisionLayers layers, const GameData *gameData, const void *self)
{
    std::unordered_set<void *> hashSet = std::unordered_set<void *>();
    float x0 = pos.x - width / 2;
    float y0 = pos.y - height / 2;

    float x1 = x0 + width;
    float y1 = y0 + height;

    // --- Enemies ---

    if ((int)layers & (int)CollisionLayers::Enemies)
    {
        for (int i = 0; i < gameData->enemies.count; i++)
        {
            Enemy *e = &gameData->enemies.enemies[i];
            // If it is dead and we aren't looking for dead ones, then we skip it. We also skip it is self and it isn't looking for self.
            if ((!((int)layers & (int)CollisionLayers::Corpses) && !e->alive) || ((int)layers & (int)CollisionLayers::Self && self == e))
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
                // It overlaps
                hashSet.insert((void *)e);
            }
        }
    }

    // --- Player ---

    // I blacked out and when I came to, the following code was just... there. - N

    // It targets a player
    if ((int)layers & (int)CollisionLayers::Player)
    {
        // Either it is able to target self of the target isn't self
        if ((((int)layers & (int)CollisionLayers::Self || self != &gameData->player)))
        {
            // It either targets corpses or the target isn't a corpse
            if (((int)layers & (int)CollisionLayers::Corpses && gameData->player.state != PlayerState::Dead))
            {
                float px0 = gameData->player.position.x - gameData->player.width / 2;
                float px1 = px0 + gameData->player.width;
                float py0 = gameData->player.position.y - gameData->player.height / 2;
                float py1 = py0 + gameData->player.height;
                // The standard overlapping square check
                if (px1 > x0 && px0 < x1 && py1 > y0 && py0 < y1)
                {
                    // It overlaps
                    hashSet.insert((void *)&gameData->player);
                }
            }
        }
    }

    // -- Projectiles --

    if ((int)layers & (int)CollisionLayers::Projectiles)
    {
        for (int i = 0; i < gameData->projectiles.count; i++)
        {

            if ((int)layers & ((int)CollisionLayers::Player | (int)CollisionLayers::Enemies))
            {
                // It cares about who's side the projectile is on, so check the team of the owner

                void *ptr = gameData->projectiles.list[i].owner;

                if (!((int)GetEntityType(ptr, gameData) & (int)layers))
                {
                    // Didn't match
                    continue;
                }
            }
            hashSet.insert((void *)&gameData->player);
        }
    }

    return hashSet;
}

// Currently only supports player, enemy, and projectile
CollisionLayers GetEntityType(const void *entity, const GameData *gameData)
{
    // Now, THIS is how you check a type: Figure out which list it is in.
    if (entity == &gameData->player)
    {
        return CollisionLayers::Player;
    }
    else if (entity >= gameData->enemies.enemies && entity < gameData->enemies.enemies + gameData->enemies.count)
    {
        return CollisionLayers::Enemies;
    }
    else if (entity >= gameData->projectiles.list && entity < gameData->projectiles.list + gameData->projectiles.count)
    {
        return CollisionLayers::Projectiles;
    }
    return (CollisionLayers)0;
};

void AttackEntity(void *entity, void *source, GameData *gameData, float damage, Vector2 force)
{
    switch (GetEntityType(entity, gameData))
    {
    case CollisionLayers::Player:
    {
        Player *ptr = (Player *)entity;
        PlayerGetHit(ptr, damage, force);
        break;
    }

    case CollisionLayers::Enemies:
    {
        // Enemy *enemy = (Enemy *)entity;
        // Make a relevant function to put it in - N
        break;
    }

    default:
        // This should never happen.
        // Maybe I should put an error message here. - N
        break;
    }
}

Vector2 GetEntityPosition(const void *entity, const GameData *gameData)
{
    switch (GetEntityType(entity, gameData))
    {
    case CollisionLayers::Player:
        return ((Player *) entity)->position;
    case ::CollisionLayers::Enemies:
        return ((Enemy *) entity)->position;
    case CollisionLayers::Projectiles:
        return ((Projectile *) entity)->position;
    default:
        return Vector2{0, 0};
    }
}
