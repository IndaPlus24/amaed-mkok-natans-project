#include "score.h"
#include "entities/enemiesStruct.h"
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>


static const std::unordered_map<EnemyType, int>  ScoreMap = { // Returns a godtyckligt value to the different enemy types
    {EnemyType::ENEMY_MELEE, 10},
    {EnemyType::ENEMY_RANGED, 5},
    {EnemyType::ENEMY_NONE, 0}
};

extern char* initials = "AAA";

extern int score = 0;

int CountDigits() {
    if (score == 0) return 4;
    return (4 - (int)log10(abs(score)));
}

void ResetScore() { //Resets score
    score = 0;
}

void IncrementScore(EnemyType enemyType) { //Increments the score based on the enemyType
    score += ScoreMap.count(enemyType) ? ScoreMap.at(enemyType) : 0;
}

int GetScore() { //OOP go brrr
    return score;
}
const char* GetFormatScore() {
    std::string unFormatScore = "Poäng: " + std::string(CountDigits(), '0');
    const char* formatScore = TextFormat("%s%i", unFormatScore.c_str(), score);
    return formatScore;
}

void ScoreDraw() {
    const int FONT_SIZE = 50;
    DrawText(GetFormatScore(), 10, GetScreenHeight() - FONT_SIZE, FONT_SIZE, WHITE);
}

void ScoreAdd() {
    std::ofstream file("scores.txt");
    if (!file.is_open()) return;

    file << initials << " " << score << "\n";

    file.close();
}

std::vector<ScoreEntry> GetHighScores(){
    std::vector<ScoreEntry> scores;
    std::ifstream file("saves/scores.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        ScoreEntry entry;
        iss >> entry.initials >> entry.score;
        if (!entry.initials.empty()) {
            scores.push_back(entry);
        }
    }

    return scores;
}

