#include "Common/GameEngine.h"

#include <iostream>

using namespace std;

int main() {

    GameEngine *engine = new GameEngine;

    try {
        engine->gameStart();
    } catch (exception &e) {
        cout << "Invalid Map provided, the game will now terminate." << endl;
    };

    return 0;
}
