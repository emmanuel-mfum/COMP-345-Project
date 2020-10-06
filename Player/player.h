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
    Player();
    int getArmies();
    int getPlayerId();
    vector<Territory*> toDefend();
    vector<Territory*> toAttack();
    void issueOrder();
    // i don't think you need this method
    void getAdjacentTerritories();
    void declareOwner(string countryName);

    // need to implement a setter method
    static void setMap(Map* worldMap);
};
