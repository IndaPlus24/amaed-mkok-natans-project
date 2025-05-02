#ifndef PLAYER_F_H
#define PLAYER_F_H

#include "gameData.h"
#include "input.h"

void PlayerUpdate(GameData *gameData, Inputs *inputs);
void PlayerDraw(Player *player);
Player CreatePlayer();

#endif