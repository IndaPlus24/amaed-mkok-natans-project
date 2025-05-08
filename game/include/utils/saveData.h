#ifndef SAVE_DATA_H
#define SAVE_DATA_H

#include "gameData.h"
#include <string>

bool SaveGameData(const GameData& gameData);

bool LoadGameData(GameData& gameData);



#endif