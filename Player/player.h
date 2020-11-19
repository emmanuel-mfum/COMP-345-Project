#pragma once

#include <vector>
#include <string>
#include "../Map/Map.h"
#include "../Order/Orders.h"

using namespace std;

class Order;
class Map;

class Player {
private:
    // declare a static Map so all players can see it
    static Map* worldMap;
    int playerId;
    int armies;
    int reinforcementPool;

    vector<Country*> ownedTerritories;

    static int playerCounter;
    static int getAndUpdateIdForNew();

public:
    //constructor
    Player();
    //destructor
    ~Player();
    //copy constructor
    Player(const Player& pl);
    friend std::ostream& operator<<(std::ostream& out, const Player& toOut);
    int getArmies();
    int getPlayerId();
    int getReinforcementPool();
    void setArmies(int army);
    void setInitialArmySize(int army);
    void setReinforcementPool(int r);
    int numOwnedCountries();
    bool deservesContinentBonus();
    void addToReinforcements(int numArmies);
    //the vector of Country pointers to defend
    vector<Country*> toDefend();
    //the vector of territory pointers to attack
    vector<Country*> toAttack();
    ////the vector of oder pointers to issueOrder
    void issueOrder();
    OrdersList *ol;
    OrdersList* getList();
    void declareOwner(string countryName);

    // need to implement a setter method
    static void setMap(Map* worldMap);
};
