#pragma once

#include <vector>
#include <string>
#include "../Map/Map.h"

using namespace std;



class Player {
private:
    int armies;
    // declare a static Map so all players can see it
    static Map* worldMap;

    int* playerId;
    int* armies;

    vector<Territory*>* ownedTerritories;



public:
    // note -- make sure to define implementation of methods in player.cpp
    Player();
    int getArmies();
    int setArmies(int sA);
    // this method can just return the list of territories the player owns!
    void toDefend();
    // this method can just return the list of territories adjacent to the territories the player owns!
    // i don't think you should pass the player as an argument, it is a member function of the player class 
    // this method just needs to loop through the list of territories the player owns, and call getAdjacentTerritories on that territory!
    // build a list of those adjacent territories and return them!

    void toAttack(Player* pl);
    void issueOrder();

    void declareOwner(string countryName);

    // need to implement a setter method
    static void setMap(Map* worldMap);
};
