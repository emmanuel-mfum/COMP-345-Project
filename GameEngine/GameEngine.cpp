
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
   vector<int>playOrder;
   vector<int>players;
	
    //still in progres All territories in the map are randomly assigned to players one by one in a round-robin fashion......
	
	
   /*the order of the player is generated randomly*/
	for (int i = 0; i < numberOfPlayers(); i++) {
	    int random = (rand())% playerList.size();
	    playOrder.push_back(players.at(random));
	}

	/*players are given a number of initial armies*/
	int armies;
	if (playerList.size() == 2) 
	{
		armies = 40;
	}
	else if (playerList.size() == 3) 
	{
		armies = 35;
	}
	else if (playerList.size() == 4)
	{
		armies = 30;
	}
	else if (playerList.size() == 5)
	{
		armies = 25;
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
