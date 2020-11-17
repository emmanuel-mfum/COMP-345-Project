
#include "GameEngine.h"
#include "MapLoader.h"
#include "Player.h"
#include "Cards.h"



using namespace std;

const char* GameEngine::directory = "../Map_Directory/";




string GameEngine::mapSelection() {
    //in progress
}


int GameEngine::numberOfPlayers() {
    //in progress
}

bool GameEngine::ObserverOption() {
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
    Player = gameEngine.Player;
    playerList = gameEngine.playerList;
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
     int numPlayers=numberOfPlayers();
    // Turn on/off observers here
     bool observerFunction = ObserverOption();

     for (int i = 1; i <= numPlayers; i++) {
         string PlayerName = "Player Number " + to_string(i);
         this->playerList.emplace_back(PlayerName);
     }
}

