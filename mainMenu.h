#ifndef H_MAIN_MENU
#define H_MAIN_MENU

#include "gameState.h"

/// @brief Sets all MM values to their defaults: call at startup and right before the menu would be exited.
void InitMM();

/// @brief Run MM. If a new state is to be entered, it will call InitMM() to reset itself and the applicable init<state> function.
/// @return The new state to enter.
GameState runMM();

#endif