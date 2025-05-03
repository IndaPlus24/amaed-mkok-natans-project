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
    player.width = 20;
    player.height = 20;
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
    DrawRectangle((int)(player->position.x - player->width / 2), (int)(player->position.y - player->height / 2), (int)player->width, (int)player->height, GREEN);
}