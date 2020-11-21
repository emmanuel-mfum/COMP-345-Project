#pragma once

#include <vector>
#include <string>
#include <map>
#include "../Map/Map.h"
#include "../Order/Orders.h"
#include "../Card/Cards.h"
#include <queue>

using namespace std;

class Order;
class OrdersList;
class Map;
class Hand;

class Player {
private:
    // declare a static Map so all players can see it
    static Map* worldMap;
    static vector<Player*>* playersInGame;
    int playerId;
    int armies;
    int reinforcementPool;
    map<int, int> deployments;

    vector<Country*> ownedTerritories;
    OrdersList* ol;
    Hand* hand;

    static int playerCounter;
    static int getAndUpdateIdForNew();
    void deployReinforcements();
    void sendToBattle(int territoryIdx, int numSlaughtering);

public:
    //constructor
    Player();
    //destructor
    ~Player();
    //copy constructor
    Player(const Player& pl);
    int getArmies();
    int getPlayerId();
    int getReinforcementPool();
    void setArmies(int army);
    void setInitialArmySize(int army);
    void setReinforcementPool(int r);
    int numOwnedCountries();
    int deservesContinentBonus();
    void addToReinforcements(int numArmies);
    //the vector of Country pointers to defend
    vector<Country*> toDefend();
    //the vector of territory pointers to attack
    vector<Country*> toAttack();
    ////the vector of oder pointers to issueOrder
    void issueOrder();
    
    OrdersList* getList();
    queue<Order*> getSortedQueue();
    void declareOwner(string countryName);
    void sustainedLosses(int numLossed);
    void wasConquered(int numLossed, string territoryName);
    void sustainOpponentLosses(int playerId, int numLossed, string territoryName);
    void conquerOpponent(int playerId, int numLossed, string territoryName);
    void clearOrders();
    void addToDeployments(int territoryIdx, int numToDeploy);
    void cleanDeploymentMap();
    void lostCountry(Country* lost);
    void validateDeployments();

    // need to implement a setter method
    static void setMap(Map* worldMap);
    static void setPlayersInGame(vector<Player*>* playersInGame);
    
};
