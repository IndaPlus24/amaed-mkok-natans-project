#include <raylib.h>
#include <raymath.h>

#include "playerFunks.h"
#include "entityFunks.h"
#include "attackFunks.h"

void PlayerFriction(Player *player)
{
    player->velocity = Vector2Lerp(player->velocity, (Vector2){0, 0}, player->friction * GetFrameTime());
}

void PlayerMovement(Player *player, const Inputs *in)
{
    Vector2 dir = {0};

    if ((int)in->up & (int)ButtonState::Down)
        dir.y -= 1.0f;
    if ((int)in->down & (int)ButtonState::Down)
        dir.y += 1.0f;
    if ((int)in->left & (int)ButtonState::Down)
        dir.x -= 1.0f;
    if ((int)in->right & (int)ButtonState::Down)
        dir.x += 1.0f;

    /* decelerate when no key is held */
    if (Vector2LengthSqr(dir) == 0.0f)
    {
        PlayerFriction(player);
        return;
    }

    dir = Vector2Normalize(dir);
    player->direction = dir;

    if (Vector2DotProduct(dir, player->velocity) >= 0 && Vector2LengthSqr(player->velocity) > player->speed * player->speed)
    {
        player->velocity = Vector2Scale(dir, player->speed);
    }
    else if (Vector2DotProduct(dir, player->velocity) < player->speed / 2)
    {
        player->velocity = Vector2Add(player->velocity, Vector2Scale(dir, player->acceleration / 2 * GetFrameTime()));
        player->velocity = Vector2ClampValue(player->velocity, 0.0f, player->speed);
    }
    else
    {
        player->velocity = Vector2Add(player->velocity, Vector2Scale(dir, player->acceleration * GetFrameTime()));
        player->velocity = Vector2ClampValue(player->velocity, 0.0f, player->speed);
    }
}

void PlayerAttack(Player *player, GameData *gameData, const Inputs *in)
{
    PlayerFriction(player);

    AttackUpdate(&player->attack, player->animationTime, player->position, player->direction, gameData);
    if (player->attack.done)
    {
        player->animationTime = 0;
        player->state = PlayerState::Neutral;
    }
}

void PlayerStartAttack(Player *player, GameData *gameData, const Inputs *in, int weapon)
{
    player->animationTime = 0;
    player->state = PlayerState::Attack;
    switch (weapon)
    {
    case 0:
        player->attack = CreateAttack(player, AttackType::testMelee);
        break;
    
    case 1:
        player->attack = CreateAttack(player, AttackType::testRanged);
        break;
    }
    PlayerAttack(player, gameData, in);
}

void PlayerUpdate(GameData *gameData, const Inputs *in)
{
    Player *player = &gameData->player;
    // Enemies *enemies = &gameData->enemies;
    // Projectiles *projectiles = &gameData->projectiles;

    switch (player->state)
    {
    case PlayerState::Neutral:
        PlayerMovement(player, in);

        if (in->a == ButtonState::Pressed)
        {
            PlayerStartAttack(player, gameData, in, 0);
        }
        else if (in->b == ButtonState::Pressed)
        {
            PlayerStartAttack(player, gameData, in, 1);
        }
        break;

    case PlayerState::Attack:
        PlayerAttack(player, gameData, in);
        break;
    case PlayerState::Dead:
        break;
    default:
        break;
    }

    Vector2 move = Vector2Scale(player->velocity, GetFrameTime());
    EntityMove(&player->position, move, player->width, player->height, gameData);
    player->animationTime += GetFrameTime();
}

void PlayerDraw(Player *player)
{
    // A "simple" "algoritm" to translate a vector into an integer representing its angle (clockwise is positive, (0,1) is 0)
    int dir = 0;
    if (player->direction.x != 0)
    {
        int a = player->direction.x > 0 ? -1 : 1;
        int b = 1;
        if (player->direction.y == 0)
        {
            b = 2;
        }
        else if (player->direction.y < 0)
        {
            b = 3;
        }
        dir = (8 + a * b) % 8;
    }
    else if (player->direction.y < 0)
    {
        dir = 4;
    }

    switch (player->state)
    {
    case PlayerState::Neutral:
        DrawRectangle((int)(player->position.x - ((player->width + 1) >> 1)), (int)(player->position.y - ((player->height + 1) >> 1)), (int)player->width, (int)player->height, GREEN);
        DrawCentre(&player->sheets[0], dir, 0, player->position);
        /* code */
        break;
    case PlayerState::Attack:
        DrawRectangle((int)(player->position.x - ((player->width + 1) >> 1)), (int)(player->position.y - ((player->height + 1) >> 1)), (int)player->width, (int)player->height, ORANGE);
        DrawCentre(&player->sheets[0], dir, 0, player->position);
        AttackDebugDraw(&player->attack, player->position, player->direction);
        break;
    case PlayerState::Dead:
        DrawRectangle((int)(player->position.x - ((player->width + 1) >> 1)), (int)(player->position.y - ((player->height + 1) >> 1)), (int)player->width, (int)player->height, DARKBLUE);
        DrawCentre(&player->sheets[0], dir, 0, player->position);
        break;
    default:
        break;
    }
}

// This fuction has been written by a human.
Player CreatePlayer(Vector2 spawnPos)
{
    Player player = {0};
    player.position = spawnPos;
    player.speed = 200.0f;
    player.acceleration = 4000.0f;
    player.friction = 25.0f;
    player.health = 250.0f;
    player.width = 14;
    player.height = 15;

    return player;
}