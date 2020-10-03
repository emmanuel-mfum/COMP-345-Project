#pragma once

#include <iostream>
#include <string>

#include "Player.h"
#include "../Map/Map.h"



using namespace std;

// initialize the static member worldMap to null
Map* Player::worldMap = nullptr;

Player::Player(){
    this->armies = 0;
    this->playerId = new int(245);
    this->ownedTerritories = new vector<Territory*>;
}

void Player::toDefend(){

}
void Player::toAttack(Player *pl){
    
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