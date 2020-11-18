
#include "GameEngine.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/player.h"
#include "../Card/Cards.h"
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

const char* GameEngine::directory = "../Map_Directory/";

int GameEngine::numberOfPlayers() {
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
string GameEngine::mapSelection() {
    vector<string> mapDirectory;
    int mapNumber = 0;
    for (const auto& mapFromDirectory : fs::directory_iterator(directory)) {
        mapDirectory.push_back(mapFromDirectory.path().filename().string());
        cout << "[ Map " << ++mapNumber << "] - Name : " << mapFromDirectory.path().filename();
        cout << endl;
    }
    int choice;
    bool flag = true;
    while (flag) {
        flag = false;
        cin.clear();
        flag&& cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Choose a map according to the map number between 1 and " << mapNumber << " :";
        cin >> choice;
        if (choice < 1 || choice > mapNumber) {
            cout << "Invalid choice, try again.";
            cout << endl;
        }
        flag = !cin.good() || choice < 1 || choice > mapNumber;
    }
    cout << "\nThe map chosen is :" << mapDirectory.at(choice - 1) << endl;
    return mapDirectory.at(choice - 1);
}

bool GameEngine::ObserverOption(string s) {
    int input;
    bool flag = true;
    bool observer = false;

    cout << "\nObserver Setting for " + s + "\n"<< endl;

    while (flag) {
        flag = false;
        cin.clear();
        flag&& cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enable the observer for " + s + " , type '1' for on and '0' for off: ";
        cin >> input;
        flag = !cin.good() || input != 1 && input != 0;
    }
    observer = input == 1;

    if (observer == true) {
        cout <<"\n"+ s + " is set to ON."<< endl;
    }
    if (observer == false) {
        cout <<"\n"+ s + " is set to OFF."<< endl;
    }
    return observer;
}




//default constructor
GameEngine::GameEngine() {
    player = new Player();
    this->playerList;
    this->playerOrder;
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
     //Getting map name
     string map= mapSelection();
    //Getting number of players
     int numPlayers=numberOfPlayers();
    //Initialize Phase Observer
     bool phaseObserverOption = ObserverOption("'Phase Observer'");
    //Initialize Statistics Observer
    bool StatisticsObserverOption = ObserverOption("'Statistics Observer'");
   
    //Creating observer objects
     if (this->phaseObserverOption) {
         phaseObserverObject = new PhaseObserver(this);
     }
     if (this->StatisticsObserverOption) {
         StatisticsObserverObject = new StatsObserver(this);
     }
	
     //Initialize map
     try {
         this->map = MapLoader::load_map(map);
     }
     catch (const exception& e) {
         cout << e.what() << endl;

         cout << "Error. Invalid map.";
         exit(0);
     }
	
     //Initialize new Deck
     this->deck = new Deck();
	
     //Initialize players
     for (int i = 1; i <= numPlayers; i++) {
         this->playerList.push_back(new Player());
     }
     
}

void GameEngine::startupPhase(){
    //still in progres All territories in the map are randomly assigned to players one by one in a round-robin fashion......
	
	
    /*the order of the player is generated randomly*/
    vector<bool> isUsed(this->playerList.size());
    for (int i = 0; i < this->playerList.size(); i++) {
        isUsed[i] = false;
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

	for (int i = 0; i < this->playerList.size(); i++) {
        while (true) {
            int random = (rand()) % this->playerList.size();
            if (isUsed[random] == false) {
                this->playerOrder.push_back(this->playerList.at(i));
                this->playerOrder.at(random)->setInitialArmySize(armies);
                isUsed[random] = true;
                break;
            }
        }
	}
    /*
    * 
    * vector<Country*> allCountries = map->getCountries(); // getCountries would be easy to implement 
      bool[size of allCountries] indexAssigned; // initially set all to false
      for (country in allCountries) {
          while (true) {
           // newIndex = generate random int (to be used as index in allCountries, indexAssigned) bounded by the number of countries
           // check if country at the newIndex is assigned
           //   if country at new index is available, assigned it to a player
           // set indexAssigned[newIndex] = true, to keep track of which countries are assigned
           // break once assigned
         }
      }
    */
    vector<Country*> allCountries = this->map->getCountries();
    vector<bool> ctryIdxAsgned;

    for (int i = 0; i < allCountries.size(); i++) {
        ctryIdxAsgned.push_back(false);
    }

    int playerIdx = 0;
    for (int i = 0; i < allCountries.size(); i++) {
        while (true) {
            int random = (rand()) % allCountries.size();
            if (ctryIdxAsgned[random] == false) {
                ctryIdxAsgned[random] = true;
                // assign to players in their original order or in their turn order?
                // assign territory to player
                this->playerList.at(playerIdx)->declareOwner(allCountries.at(random)->getTerritoryName());
                playerIdx = (playerIdx + 1) % this->playerList.size();
                break;
            }
        }
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
