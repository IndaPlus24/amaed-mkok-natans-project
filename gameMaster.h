#ifndef H_GAME
#define H_GAME

#include "gameState.h"

/// @brief Contains all data necessary for GM initialisation
typedef struct
{

} dataGM;

/// @brief Initialise GM.
/// @param initData The data which will be used as a base
void InitGM(dataGM initData);

/// @brief Run game logic (aka GameMaster).
/// @return The next game state to enter
GameState RunGM();

#endif