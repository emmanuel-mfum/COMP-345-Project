#pragma once

#include <iostream>
#include <string>

#include "player.h"
#include "../Map/Map.h"



using namespace std;

// initialize the static member worldMap to null
Map* Player::worldMap = nullptr;

Player::Player(){
    this->armies = 0;
    this->playerId = new int(245);
    this->ownedTerritories = new vector<Territory*>;   
}
int Player::getArmies(){
   this->armies;
}

int Player::getPlayerId(){
    this->playerId;
}


// this method can just return the list of territories the player owns!
vector<Territory*> Player::toDefend(){
   cout<<"The list of territories owned by the player "<<playerId<<":"<<endl;

   return *this->ownedTerritories;
}
/*
* this method can just return the list of territories adjacent to the territories the player owns!
* i don't think you should pass the player as an argument, it is a member function of the player class 
* this method just needs to loop through the list of territories the player owns, and call getAdjacentTerritories on that territory!
* build a list of those adjacent territories and return them!
*/

vector<Territory*> Player::toAttack(){
    // build a list of territories
    vector<Territory*> toAttack = vector<Territory*>();

    // loop through each owned territory
    for (int i = 0; i < this->ownedTerritories->size(); i++){
        // get the adjacent territories to the current owned territory
        vector<Territory*> adjacentTerrs = *this->ownedTerritories->at(i)->getAdjacentTerritories();
        // loop through each adjacent territories
        for (int j = 0; j < adjacentTerrs.size(); j++) {
            bool found = false;
            // for each territory that has been added to toAttack
            for (int k = 0; k < toAttack.size(); k++) {
                // make sure the current adjacentTerritory is not already in the list
                if (toAttack.at(k)->getTerritoryName().compare(adjacentTerrs.at(j)->getTerritoryName()) == 0) {
                    found = true;
                    break;
                }
            }
            // if the territoryname wasn't found
            if (!found) {
                toAttack.push_back(adjacentTerrs.at(j));
            }
        }
    }
    // return a list of unique adjacent territories to the players territory
    return toAttack;
}

void Player::declareOwner(string countryName) {
    // let the map know whtat the player now owns the territory identified by countryName
    Map* m = worldMap;

    Territory* ownedTerritory = worldMap->setPlayerOwnership(*this->playerId, countryName);

    // HERE: just need to add ownedTerritory to this players list of owned territories

}




// sets the map object shared by all players that the player objects can use to declare ownership of territories
void Player::setMap(Map* map) {
    // inside member functions, the players can now access worldMap to do things to territories and stuff
    Player::worldMap = map;
}