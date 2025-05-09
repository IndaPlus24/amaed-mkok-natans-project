#include "saveData.h"
#include <fstream>
#include <iostream>

using namespace std;
bool SaveGameData(const GameData& gameData, const string& filename) {
    ofstream out(filename, ios::binary);

    if (!out) {
        cerr << "Failed to open for saving." << endl;
        return false;
    }

    //Save player data

    out.close();


}