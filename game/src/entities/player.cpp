#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "playerFunks.h"
#include "entityFunks.h"
#include "attackFunks.h"

#define INVINCIBILITY_DURATION 2.0f
#define STUN_DURATION 0.4f

bool isInDoor = false;

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

void PlayerNeutral(Player *player, GameData *gameData, const Inputs *in)
{
    PlayerMovement(player, in);

    if (in->a == ButtonState::Pressed)
    {
        PlayerStartAttack(player, gameData, in, 0);
    }
    else if (in->b == ButtonState::Pressed)
    {
        PlayerStartAttack(player, gameData, in, 1);
    }
}

void PlayerUpdate(GameData *gameData, const Inputs *in)
{
    Player *player = &gameData->player;

    player->animationTime += GetFrameTime();

    if (player->invincibilityTimer > 0)
    {
        player->invincibilityTimer -= GetFrameTime();
    }

    switch (player->state)
    {
    case PlayerState::Neutral:
        PlayerNeutral(player, gameData, in);
        break;

    case PlayerState::Attack:
        PlayerAttack(player, gameData, in);
        break;

    case PlayerState::Stunned:
        if (player->animationTime >= STUN_DURATION)
        {
            player->state = PlayerState::Neutral;
            player->animationTime = 0.0f;
            PlayerNeutral(player, gameData, in);
        }
        else
        {
            PlayerFriction(player);
        }
        break;

    case PlayerState::Dead:
        PlayerFriction(player);
        break;
    }

    Vector2 move = Vector2Scale(player->velocity, GetFrameTime());
    EntityMove(&player->position, move, player->width, player->height, gameData);
    // If this doesn't work it will be hard to debug :)
    // Could implement a version of this that doesnt use fixed array indeces and instead loops through all possible doors in the room but this works for now since we only have 2 doors maximum ATM.
    if (((int)(player->position.x/tileSize) == gameData->currentRoom->doors[0].posX && (int)(player->position.y/tileSize) == gameData->currentRoom->doors[0].posY) && !isInDoor)
    {
        isInDoor = true;
        player->position.x = gameData->currentRoom->doors[0].linkedDoor->posX * tileSize;
        player->position.y = gameData->currentRoom->doors[0].linkedDoor->posY * tileSize;
        gameData->currentRoom = &gameData->map.rooms[gameData->currentRoom->doors[0].toRoomId];
        gameData->enemies = gameData->map.enemies[gameData->currentRoom->doors[0].toRoomId];
    }
    else if (((int)(player->position.x/tileSize) == gameData->currentRoom->doors[1].posX && (int)(player->position.y/tileSize) == gameData->currentRoom->doors[1].posY) && !isInDoor)
    {
        isInDoor = true;
        player->position.x = gameData->currentRoom->doors[1].linkedDoor->posX * tileSize;
        player->position.y = gameData->currentRoom->doors[1].linkedDoor->posY * tileSize;
        gameData->currentRoom = &gameData->map.rooms[gameData->currentRoom->doors[1].toRoomId];
        gameData->enemies = gameData->map.enemies[gameData->currentRoom->doors[1].toRoomId];
    }
    else if (isInDoor && !((player->position.x == gameData->currentRoom->doors[0].posX * tileSize && player->position.y == gameData->currentRoom->doors[0].posY * tileSize) || (player->position.x == gameData->currentRoom->doors[1].posX * tileSize && player->position.y == gameData->currentRoom->doors[1].posY * tileSize)))
    {
        isInDoor = false;
    }
}

void PlayerDraw(Player *player)
{
    if (player->invincibilityTimer > 0.0f && ((int)(GetTime() * 10.0)) & 1)
    {
        return;
    }

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
        DrawRectangle((int)(player->position.x - ((player->width) >> 1)), (int)(player->position.y - ((player->height) >> 1)), (int)player->width, (int)player->height, GREEN);
        DrawCentre(&player->sheets[0], dir, 0, player->position);
        /* code */
        break;
    case PlayerState::Attack:
        DrawRectangle((int)(player->position.x - ((player->width) >> 1)), (int)(player->position.y - ((player->height) >> 1)), (int)player->width, (int)player->height, ORANGE);
        DrawCentre(&player->sheets[0], dir, 0, player->position);
        AttackDebugDraw(&player->attack, player->position, player->direction);
        break;
    case PlayerState::Stunned:
        DrawRectangle((int)(player->position.x - ((player->width) >> 1)), (int)(player->position.y - ((player->height) >> 1)), (int)player->width, (int)player->height, RED);
        DrawCentre(&player->sheets[0], dir, 0, player->position);
        break;
    case PlayerState::Dead:
        DrawRectangle((int)(player->position.x - ((player->width) >> 1)), (int)(player->position.y - ((player->height) >> 1)), (int)player->width, (int)player->height, DARKBLUE);
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
    player.direction = Vector2{0.0f, 1.0f};
    player.speed = 200.0f;
    player.acceleration = 2000.0f;
    player.friction = 15.0f;
    player.health = 250.0f;
    player.width = 14;
    player.height = 16;
    player.invincibilityDuration = 2.0f;
    player.invincibilityTimer = 0;

    return player;
}

void PlayerGetHit(Player *player, float damage, Vector2 force)
{
    if (player->invincibilityTimer > 0)
        return;

    player->health -= damage;
    player->velocity = Vector2Add(player->velocity, force);

    if (player->state == PlayerState::Attack)
    {
        AttackForceEnd(&player->attack);
    }

    if (player->health <= 0)
    {
        player->state = PlayerState::Dead;
        return;
    }

    player->state = PlayerState::Stunned;
    player->invincibilityTimer = INVINCIBILITY_DURATION;

    player->animationTime = 0.0f;
}
