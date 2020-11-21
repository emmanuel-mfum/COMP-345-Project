
#include "GameEngine.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/player.h"
#include "../Card/Cards.h"
#include "../Observer/Observer.h"
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <queue>

namespace fs = std::filesystem;
using namespace std;

class GameEngine;


const char* GameEngine::directory = "../Map_Directory/";


ostream& operator<<(ostream& out, Phases e) {
    switch (e) {
    case Phases::NIL:
        out << "Set-Up";
        break;
    case Phases::Reinforcement:
        out << "Reinforcement";
        break;
    case Phases::IssueOrder:
        out << "Issue-Order";
        break;
    default:
        out << "Execute-Order";
        break;
    }
    return out;
}



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
    this->playerList;
    this->playerOrder;
    this->currentPhase = Phases::NIL;
    this->gsObs = nullptr;
    this->phaseObs = nullptr;
    this->currentPlayerIdx = 0;
    this->currentPlayerReinf = 0;
    this->startPlayerExecute = true;
    this->map = nullptr;
    //map = new Map(); default constructor needed
    deck = new Deck();
}

//copy constructor
GameEngine::GameEngine(const GameEngine& gameEngine) {
    playerList = gameEngine.playerList;
    map = gameEngine.map;
    deck = gameEngine.deck;
    this->currentPhase = gameEngine.currentPhase;
    this->gsObs = gameEngine.gsObs;
    this->phaseObs = gameEngine.phaseObs;
    this->currentPlayerIdx = gameEngine.currentPlayerIdx;
    this->currentPlayerReinf = gameEngine.currentPlayerReinf;
    this->startPlayerExecute = gameEngine.startPlayerExecute;
}

//destructor
GameEngine::~GameEngine() {
    delete this->map;
    delete this->deck;
    delete this->gsObs;
    delete this->phaseObs;
}

Phases GameEngine::getCurrentPhase() {
    return this->currentPhase;
}

int GameEngine::getPlayerAtCurIdx() {
    return this->playerOrder.at(this->currentPlayerIdx)->getPlayerId();
}

int GameEngine::getCurrentReinf() {
    return this->currentPlayerReinf;
}

/*
* For the ObserverDriver;
*/
void GameEngine::smallGameStart(Map* map, vector<Player*> playerOrder) {
    this->map = map;
    Player::setMap(this->map);
    this->gsObs = new GameStatsObserver(this->map);
    this->map->attachGameStatsObserver(this->gsObs);

    this->phaseObs = new PhaseObserver(this);
    //Initialize new Deck
    this->deck = new Deck();

    //Initialize players
    this->playerOrder = playerOrder;
}


void GameEngine::gameStart() {
     //Getting map name
     string map= mapSelection();
     //Getting number of players
     int numPlayers=numberOfPlayers();
     //Initialize Phase Observer
     bool phaseObserverOption = ObserverOption("'Phase Observer'");
     //Initialize Statistics Observer
     bool statisticsObserverOption = ObserverOption("'Statistics Observer'");
	
     //Initialize map
     try {
         this->map = MapLoader::load_map(GameEngine::directory + map);
     }
     catch (const exception& e) {
         cout << e.what() << endl;

         cout << "Error. Invalid map.";
         exit(0);
     }

     Player::setMap(this->map);
     this->gsObs = new GameStatsObserver(this->map);
     this->map->attachGameStatsObserver(this->gsObs);

     this->phaseObs = new PhaseObserver(this);

     //Creating observer objects
     if (!phaseObserverOption) {
         this->phaseObs->turnOff();
     }

     if (!statisticsObserverOption) {
         this->gsObs->turnOff();
     }
	
     //Initialize new Deck
     this->deck = new Deck();
     this->deck->shuffleDeck();
	
     //Initialize players
     for (int i = 1; i <= numPlayers; i++) {
         this->playerList.push_back(new Player());
     }
     
     Player::setPlayersInGame(&this->playerList);
}

void GameEngine::startupPhase(){
   
    /*the order of the player is generated randomly*/
    vector<bool> isUsed(this->playerList.size());
    for (int i = 0; i < this->playerList.size(); i++) {
        isUsed[i] = false;
    }

    /*players are given a number of initial armies*/
    int armies;
    if (playerList.size() == 2) {
        armies = 40;
    }
    else if (playerList.size() == 3) {
        armies = 35;
    }
    else if (playerList.size() == 4) {
        armies = 30;
    }
    else {
        armies = 25;
    }

	for (int i = 0; i < this->playerList.size(); i++) {
        while (true) {
            int random = (rand()) % this->playerList.size();
            if (isUsed[random] == false) {
                this->playerOrder.push_back(this->playerList.at(random));
                this->playerOrder.at(i)->setInitialArmySize(armies);
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
                this->playerOrder.at(playerIdx)->declareOwner(allCountries.at(random)->getTerritoryName());
                playerIdx = (playerIdx + 1) % this->playerOrder.size();
                break;
            }
        }
    }
}

void  GameEngine::issueOrdersPhase(){
    this->currentPhase = Phases::IssueOrder;
    this->phaseObs->update();
    for (int i = 0; i < this->playerOrder.size(); i++) {
        this->playerOrder[i]->issueOrder();
        this->currentPlayerIdx = i;
        this->phaseObs->update();
    }
};

void GameEngine::reinforcementPhase(){
    this->currentPhase = Phases::Reinforcement;
    this->phaseObs->update();

    int minArmies = 3;
    
    for (int i = 0; i < playerOrder.size(); i++) {
        // get num based on current num countries
        int armies = this->playerOrder.at(i)->numOwnedCountries() / 3;
        // get min
        armies = (armies > 3) ? armies : minArmies;
        // add bonus if applicable
        armies += this->playerOrder.at(i)->deservesContinentBonus();
        // add to reinforcement pool
        this->playerOrder.at(i)->addToReinforcements(armies);
        this->currentPlayerReinf = armies;
        this->currentPlayerIdx = i;
        this->phaseObs->update();
    }
};


void GameEngine::executeOrdersPhase(){
    this->currentPhase = Phases::ExecuteOrder;
    this->phaseObs->update();
    for (int i =0; i < this->playerOrder.size(); i++){
        this->currentPlayerIdx = i;
        this->phaseObs->update();
        this->startPlayerExecute = false;

        vector<queue<Order*>> playerOrders;
        for (Player* player : this->playerOrder) {
            playerOrders.push_back(player->getSortedQueue());
            player->clearOrders();
        }

        vector<bool> deploysFinished;
        for (int i = 0; i < this->playerOrder.size(); i++) {
            deploysFinished.push_back(false);
        }
        int roundRobinIdx = 0;

        // consume all deploys in round robin fashion
        while (true) {
            if (
                !deploysFinished[roundRobinIdx] && 
                playerOrders[roundRobinIdx].size() > 0 && 
                playerOrders[roundRobinIdx].front()->getType().compare("Deploy") == 0
            ) {
                Order* deployOrder = playerOrders[roundRobinIdx].front();
                deployOrder->execute();
                playerOrders[roundRobinIdx].pop();
            }
            else if (!deploysFinished[roundRobinIdx]) {
                deploysFinished[roundRobinIdx] = true;
            }
            bool breakNeeded = false;
            for (bool b : deploysFinished) {
                if (b) {
                    breakNeeded = true;
                    break;
                }
            }
            if (breakNeeded) {
                break;
            }
            roundRobinIdx = (roundRobinIdx + 1) % playerOrders.size();
        }
        roundRobinIdx = 0;
        // consume all remaining orders
        while (true) {
            if (playerOrders[roundRobinIdx].size() > 0) {
                Order* deployOrder = playerOrders[roundRobinIdx].front();
                deployOrder->execute();
                playerOrders[roundRobinIdx].pop();
            }
            bool breakNeeded = true;
            for (auto orderQueue : playerOrders) {
                if (orderQueue.size() > 0) {
                    breakNeeded = false;
                    break;
                }
            }
            if (breakNeeded) {
                break;
            }
            roundRobinIdx = (roundRobinIdx + 1) % playerOrders.size();
        }
        
        this->startPlayerExecute = true;
    }
}

void GameEngine::mainGameLoop() {
    bool gameover = false;

    while (!gameover) {
        gameover = true;
        this->reinforcementPhase();
        this->issueOrdersPhase();
        this->executeOrdersPhase();
        //Still coding
    }
}
