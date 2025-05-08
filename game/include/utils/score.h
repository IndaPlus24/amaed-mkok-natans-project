#ifndef SCORE_H
#define SCORE_H

#include <string>
#include <vector>
#include "gameData.h"
#include "enemiesStruct.h"

extern int score;

struct ScoreEntry {
    std::string initials;
    int score;
};
// Resets current in-game score to zero
void ResetScore();

// Increments score based on enemy type
void IncrementScore(EnemyType enemyType);

// Returns current in-game score
int GetScore();

// Returns the top score from the high score list
int GetHighScore();

// Renders score on screen in left bottom screen when in-game
void ScoreDraw();

// Returns a formatted score string for drawing
const char* GetFormatScore();

// Returns the list of high scores
std::vector<ScoreEntry> GetHighScores();

// Adds a score entry with initials to the high score list (Saves it)
void ScoreAdd(const std::string& initials, int score);

//Loads the existing scores
void ScoreLoad();

#endif