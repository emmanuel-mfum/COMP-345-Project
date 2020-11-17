
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
    cout << "============= Number Of Players =============" << endl;
    int numOfPlayer;
    bool flag = true;
    while (flag) {
        flag = false;
        cin.clear();
        flag&& cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter the amount of players (2-5):";
        cin >> numOfPlayer;

        if (numOfPlayer < 2 || numOfPlayer > 5) {
            cout << "Wrong amount try again" << endl;
        }
        flag = !cin.good() || numOfPlayer < 2 || numOfPlayer > 5;
    }
    cout << "\nYou have selected " << numOfPlayer << endl;
    return numOfPlayer;
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

void  GameEngine::issueOrdersPhase(Player* playerList){
    
        for (int i = 0; i < playerList.size(); i++)
        {
            playerList[i]->issueOrder();
        }
};

void GameEngine::reinforcementPhase(Player* playerList){
    int bonus;
    int armies;
       
    armies=3;
    for (int i = 0; i < playerList.size(); i++)
    {
        bonus= (int) (playerList->ownedTerritories/3);
             
        //calcuate the contient bouns
        //Still figuring how to check player owns all territories
        //if (){
        //
        /}
        armies +=bonus;
        playerList->setsetReinforcementPool(armies);
    }
};

void GameEngine::mainGameLoop() {
    bool Gameover=true;
    
    while (Gameover){
        Gameover=false;
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
        //Still coding
    }
}
