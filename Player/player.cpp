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
void Player::toDefend(){
   cout<<"The list of territories owned by the player "<<playerId<<":"<<endl;
   this->ownedTerritories;
}
    /*
     *this method can just return the list of territories adjacent to the territories the player owns!
     *i don't think you should pass the player as an argument, it is a member function of the player class 
     *this method just needs to loop through the list of territories the player owns, and call getAdjacentTerritories on that territory!
     *build a list of those adjacent territories and return them!
     */
void Player::toAttack(){
    
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