#include "playerFunks.h"
#include "entityFunks.h"

Player CreatePlayer()
{
    Player player;
    player.acceleration = 1;
    player.speed = 128;
    player.health = 20;
    player.animationTime = 0;
    player.state = PlayerState::Neutral;
    player.position = Vector2{0, 0};
    player.direction = Vector2{0, 1};
    player.width = 14;
    player.height = 15;
    return player;
}

void PlayerNeutral(Player *player, GameData *gameData, Inputs *inputs)
{
    Vector2 inputDir = Vector2{0, 0};

    if ((int)inputs->up & (int)ButtonState::Down)
    {
        inputDir = Vector2Add(inputDir, Vector2{0, -1});
    }
    if ((int)inputs->down & (int)ButtonState::Down)
    {
        inputDir = Vector2Add(inputDir, Vector2{0, 1});
    }
    if ((int)inputs->left & (int)ButtonState::Down)
    {
        inputDir = Vector2Add(inputDir, Vector2{-1, 0});
    }
    if ((int)inputs->right & (int)ButtonState::Down)
    {
        inputDir = Vector2Add(inputDir, Vector2{1, 0});
    }

    if (inputDir.x != 0 || inputDir.y != 0)
    {
        player->direction = inputDir;
        inputDir = Vector2Normalize(inputDir);
    }

    player->velocity = Vector2Scale(inputDir, player->speed);
}

void PlayerUpdate(GameData *gameData, Inputs *inputs)
{
    Player *player = &gameData->player;

    switch (player->state)
    {
    case PlayerState::Neutral:
        PlayerNeutral(player, gameData, inputs);
        break;

    default:
        break;
    }

    Vector2 move = Vector2Scale(player->velocity, GetFrameTime());
    EntityMove(&player->position, move, player->width, player->height, gameData);
}

void PlayerDraw(Player *player)
{
    DrawRectangle((int)(player->position.x - ((player->width + 1) >> 1)), (int)(player->position.y - ((player->height + 1) >> 1)), (int)player->width, (int)player->height, GREEN);

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
    DrawCentre(&player->sheets[0], dir, 0, player->position);
}