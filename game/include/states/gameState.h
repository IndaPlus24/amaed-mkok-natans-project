#ifndef H_GAME_STATE
#define H_GAME_STATE

enum class GameState
{
    // Main menu, aka MM, or Menu Master
    MainMenu,
    // The game itself, aka GM
    GameMaster,
    // I might get rid of this one
    GameOver,
    // Break the main loop
    Break,
    //High Score page
    HighScore,
};

#endif