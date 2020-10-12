#pragma once

#include <vector>
#include <string>
#include "../Map/Map.h"

using namespace std;



class Player {
private:
    // declare a static Map so all players can see it
    static Map* worldMap;
    int* playerId;
    int* armies;

    vector<Territory*>* ownedTerritories;



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
    //the vector of territory pointers to defend
    vector<Territory*> toDefend();
    //the vector of territory pointers to attack
    vector<Territory*> toAttack();
    ////the vector of oder pointers to issueOrder
    vector<Order*> issueOrder();
    OrdersList *ol;
   
    void declareOwner(string countryName);

    // need to implement a setter method
    static void setMap(Map* worldMap);
};
