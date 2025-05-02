#include "playerFunks.h"

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

void PlayerMove(Player *player, GameData *gameData)
{
    Vector2 move = Vector2Scale(player->velocity, GetFrameTime());

    // Horizontal
    if (move.x != 0)
    {
        int sign = move.x > 0 ? 1 : -1;
        float margin = sign * player->width / 2;

        int y0 = (int)((player->position.y - player->height / 2) / tileSize);     // y start
        int y1 = (int)((player->position.y + player->height / 2 - 1) / tileSize); // y end
        int x0 = (int)((player->position.x) / tileSize);                          // x start
        int x1 = (int)((player->position.x + move.x + margin) / tileSize);        // x limit

        // Did it hit a wall?
        bool stop = false;

        // loop from x0 to x1 (inclusive)
        for (int x = x0; x != x1 + sign; x += sign)
        {
            for (int y = y0; y <= y1; y++)
            {
                if (!GetTile(&gameData->currentRoom, x, y).isWalkable)
                {
                    stop = true;
                    break;
                }
            }
            if (stop)
            {
                int lastTile = x - sign; // The x of the tile they would have ended up in

                player->position.x = (lastTile + (sign + 1) / 2) * tileSize - margin;
                break;
            }
        }

        if (!stop)
        {
            player->position.x += move.x;
        }
    }

    // Vertical
    if (move.y != 0)
    {
        int sign = move.y > 0 ? 1 : -1;
        float margin = sign * player->width / 2;

        int y0 = (int)((player->position.y) / tileSize);                         // y start
        int y1 = (int)((player->position.y + move.y + margin) / tileSize);       // y limit
        int x0 = (int)((player->position.x - player->width / 2) / tileSize);     // x start
        int x1 = (int)((player->position.x + player->width / 2 - 1) / tileSize); // x end

        // Did it hit a wall?
        bool stop = false;

        // loop from y0 to y1 (inclusive)
        for (int y = y0; y != y1 + sign; y += sign)
        {
            for (int x = x0; x <= x1; x++)
            {
                if (!GetTile(&gameData->currentRoom, x, y).isWalkable)
                {
                    stop = true;
                }
            }
            if (stop)
            {
                int lastTile = y - sign; // The y of the tile they would have ended up in

                player->position.y = (lastTile + (sign + 1) / 2) * tileSize - margin;
                break;
            }
        }

        if (!stop)
        {
            player->position.y += move.y;
        }
    }
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

    PlayerMove(player, gameData);
}

void PlayerDraw(Player *player)
{
    DrawRectangle((int)(player->position.x - player->width / 2), (int)(player->position.y - player->height / 2), (int)player->width, (int)player->height, GREEN);
}