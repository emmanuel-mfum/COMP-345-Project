
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
    player = new Player();
    playerList = list<Player>();
    //map = new Map(); default constructor needed
    deck = new Deck();
}

//copy constructor
GameEngine::GameEngine(const GameEngine& gameEngine) {
    player = gameEngine.player;
    playerList = gameEngine.playerList;
    map = gameEngine.map;
    deck = gameEngine.deck;
}

//destructor
GameEngine::~GameEngine() {
    delete this->player;
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

void GameEngine::startupPhase(){
    //still in progres.....
    int numberOfArmies;
		if (numberOfPlayers() == 2) {
			numberOfArmies = 40;
		}
		else if (numberOfPlayers() == 3) {
			numberOfArmies = 35;
		}
		else if (numberOfPlayers() == 4) {
			numberOfArmies = 30;
		}	
		else if (numberOfPlayers() == 5) {
			numberOfArmies = 25;
	}
}
