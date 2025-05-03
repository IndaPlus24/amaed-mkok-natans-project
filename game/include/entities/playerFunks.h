#ifndef PLAYER_F_H
#define PLAYER_F_H

#include "gameData.h"
#include "input.h"

void PlayerUpdate(GameData *gameData, const Inputs *in);
void PlayerDraw(Player *player);
Player CreatePlayer(Vector2 spawnPos);

#endif