#pragma once

#include <iostream>
#include <string>

#include "player.h"
#include "../Order/Orders.h"
#include "../Map/Map.h"



using namespace std;

// initialize the static member worldMap to null
Map* Player::worldMap = nullptr;

int Player::playerCounter = 0;

Player::Player(){
    this->armies = 0;
    this->playerId = Player::getAndUpdateIdForNew();
    this->ownedTerritories;   
    OrdersList* ol = new OrdersList();
}
int Player::getArmies(){
   return this->armies;
}

int Player::getPlayerId(){
    return this->playerId;
}

int Player::getReinforcementPool() {

    return this->reinforcementPool;
}

void Player::setInitialArmySize(int size) {
    this->armies = size;
    this->reinforcementPool = size;
}

void Player::setArmies(int army)
{
    this->armies = army;
}

void Player::setReinforcementPool(int r) {
    this->reinforcementPool = r;
}

int Player::getAndUpdateIdForNew() {
    int newInt = Player::playerCounter;
    Player::playerCounter++;
    return newInt;
}


// this method can just return the list of territories the player owns!
vector<Country*> Player::toDefend(){
   cout<<"The list of territories owned by the player "<<playerId<<":"<<endl;

   return this->ownedTerritories;
}

vector<Country*> Player::toAttack(){
    // build a list of territories
    vector<Country*> toAttack = vector<Country*>();

    // loop through each owned territory
    for (int i = 0; i < this->ownedTerritories.size(); i++){
        // get the adjacent territories to the current owned territory
        vector<Country*> adjacentTerrs = (dynamic_cast<MapComponent*>(this->ownedTerritories.at(i)))->getAdjacentCountries();
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
//creates an order object and adds it to the list of orders
void Player::issueOrder(int num){
    switch (num)
    {
        case 1: {
            Deploy deploy=Deploy("deploy");
            ol->addOrder(deploy);
            cout<< "Deployed"<<endl;
            break;
        }
        case 2: {
            Advance advance= Advance("advance");
            ol->addOrder(advance);
            cout<< "Advanced"<<endl;
            break;
        }
        case 3:{
            Bomb bomb=Bomb("bomb");
            ol->addOrder(bomb);
            cout<<"Bomb"<<endl;
            break;
        }
        case 4: {
            Blockade blockade=Blockade("blockade");
            ol->addOrder(blockade);
            cout<< "Blockade"<<endl;
            break;
        }
        case 5:{
            Airlift airlift=Airlift("airlift");
            ol->addOrder(airlift);
            cout<<"Airlift"<<endl;
            break;
        }
        case 6:{
            Negotiate negotiate=Negotiate("Negotaite");
            ol->addOrder(negotiate);
            cout<<"Negotiate"<<endl;
            break;
        }
    }
}



void Player::declareOwner(string countryName) {
    // let the map know whtat the player now owns the territory identified by countryName
    Map* m = worldMap;

    Country* ownedTerritory = worldMap->setPlayerOwnership(this->playerId, countryName);

    // HERE: just need to add ownedTerritory to this players list of owned territories
    this->ownedTerritories.push_back(ownedTerritory);
}




// sets the map object shared by all players that the player objects can use to declare ownership of territories
void Player::setMap(Map* map) {
    // inside member functions, the players can now access worldMap to do things to territories and stuff
    Player::worldMap = map;
}

int Player::numOwnedCountries() {
    return this->ownedTerritories.size();
}

bool Player::deservesContinentBonus() {
    return Player::worldMap->deservesContinentBonus(this->playerId);
}

void Player::addToReinforcements(int numArmies) {
    this->armies += numArmies;
    this->reinforcementPool += numArmies;
}
