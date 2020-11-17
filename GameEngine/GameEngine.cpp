#include "GameEngine.h"
#include "MapLoader.h"
#include "Player.h"
#include "Cards.h"

#include <iostream>

using namespace std;

const char* GameEngine::directory = "../Map_Directory/";




string GameEngine::mapSelection() {
    //in progress
}


int GameEngine::playerSelection() {
    //in progress
}

bool GameEngine::ObserverFunction() {
    //in progress
}

//default constructor
GameEngine::GameEngine() {
    Player = new Player();
    playerList = list<Player>();
    map = new Map(); 
    deck = new Deck();
}

//copy constructor
GameEngine::GameEngine(const GameEngine& gameEngine) {
    currentPlayer = gameEngine.currentPlayer;
    players = gameEngine.players;
    map = gameEngine.map;
    deck = gameEngine.deck;
}

//destructor
GameEngine::~GameEngine() {
    delete this->Player;
    delete this->map;
    delete this->deck;
}



void GameEngine::gameStart() {
    // choose map
     string map= mapSelection();
    // Select number of players 2-5
     int numberOfPlayers=playerSelection();
    // Turn on/off observers here
     bool observerFunction = ObserverFunction();
}

