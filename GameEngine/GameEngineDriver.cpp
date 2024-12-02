#include "GameEngine.h"

#include <iostream>

using namespace std;

int main() {

    GameEngine* engine = new GameEngine;

    try {
        engine->gameStart();
        engine->startupPhase();
        engine->mainGameLoop();
    }
    catch (exception& ex) {
        cout << "Game was not able to start. Game will terminate now";
    };

    return 0;
}
