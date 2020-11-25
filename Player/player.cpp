#pragma once

#include <iostream>
#include <string>

#include "player.h"
#include "../Order/Orders.h"
#include "../Map/Map.h"
#include "../Card/Cards.h"

#include <queue>
#include <time.h>


using namespace std;

// initialize the static member worldMap to null
Map* Player::worldMap = nullptr;
vector<Player*>* Player::playersInGame = nullptr;
Deck* Player::gameDeck = nullptr;

int Player::playerCounter = 0;

Player::Player(){
    this->armies = 0;
    this->reinforcementPool = 0;
    this->playerId = Player::getAndUpdateIdForNew();
    this->ownedTerritories;   
    this->ol = new OrdersList();
    this->hand = new Hand();
    this->deployments;
    this->strategy = new PlayerStrategy(this);
}

Player::Player(int strategyNum) {
    this->armies = 0;
    this->reinforcementPool = 0;
    this->playerId = Player::getAndUpdateIdForNew();
    this->ownedTerritories;
    this->ol = new OrdersList();
    this->hand = new Hand();
    this->deployments;
    
    switch (strategyNum) {
    case 1:
        this->strategy = new HumanPlayerStrategy(this);
        break;
    case 2:
        this->strategy = new BenevolentPlayerStrategy(this);
        break;
    case 3:
        this->strategy = new NeutralPlayerStrategy(this);
        break;
    case 4:
        this->strategy = new AggressivePlayerStrategy(this);
        break;
    case 5:
    default:
        this->strategy = new PlayerStrategy(this);
    }
}

Player::Player(const Player& pl) {
    this->armies = pl.armies;
    this->playerId = Player::getAndUpdateIdForNew();
    this->ownedTerritories;
    for (int i = 0; i < pl.ownedTerritories.size(); i++) {
        this->ownedTerritories.push_back(pl.ownedTerritories[i]);
    }
    this->ol = new OrdersList();
    this->hand = new Hand();
    this->strategy = new PlayerStrategy(this);
}



Player::~Player() {}

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
    return this->strategy->toDefend();
}

vector<Country*> Player::toAttack(){
    return this->strategy->toAttack();
}
//creates an order object and adds it to the list of orders
void Player::issueOrder() {
    this->strategy->issueOrder();
}

void Player::validateDeployments() {
    for (Country* c : this->ownedTerritories) {
    }
}

void Player::sendToBattle(int territoryIdx, int numSlaughtering) {
    // send to slaughter
    this->deployments[territoryIdx] -= numSlaughtering;
    // remove if 0
    if (this->deployments[territoryIdx] <= 0) {
        this->deployments.erase(this->deployments.find(territoryIdx));
    }
}

void Player::addToDeployments(int territoryIdx, int numToDeploy) {

    if (this->deployments.find(territoryIdx) == this->deployments.end()) {
        // no armies deployed yet on this territory
        this->deployments.insert(pair<int, int>(territoryIdx, numToDeploy));
    }
    else {
        // add to current deployment
        this->deployments[territoryIdx] += numToDeploy;
    }
    int total = 0;
    for (auto it = this->deployments.begin(); it != this->deployments.end(); ++it) {
        total += it->second;
    }
}

void Player::cleanDeploymentMap() {
    this->deployments.clear();
    int totalArmiesRemaining = 0;
    for (Country* c : this->ownedTerritories) {
        if (c->getArmiesOnTerritory() > 0) {
            totalArmiesRemaining += c->getArmiesOnTerritory();
            this->deployments.insert(pair<int, int>(c->getCountryId(), c->getArmiesOnTerritory()));
        }
    }
    
    this->armies = totalArmiesRemaining + this->reinforcementPool;
}

void Player::sustainedLosses(int numLossed) {
    this->armies -= numLossed;
}

void Player::wasConquered(int numLossed, string territoryName) {
    this->sustainedLosses(numLossed);
    // remove the territory from the player's owned territories
    vector<Country*>::iterator it;
    for (it = this->ownedTerritories.begin(); it != this->ownedTerritories.end(); ++it) {
        if ((*it)->getTerritoryName().compare(territoryName) == 0) {
            break;
        }
    }
    this->ownedTerritories.erase(it);
}

void Player::sustainOpponentLosses(int playerId, int numLossed, string territoryName) {
    for (int i = 0; i < Player::playersInGame->size(); i++) {
        if (Player::playersInGame->at(i)->getPlayerId() == playerId) {
            Player::playersInGame->at(i)->sustainedLosses(numLossed);
            break;
        }
    }
}

void Player::conquerOpponent(int playerId, int numLossed, string territoryName) {
    for (int i = 0; i < Player::playersInGame->size(); i++) {
        if (Player::playersInGame->at(i)->getPlayerId() == playerId) {
            Player::playersInGame->at(i)->wasConquered(numLossed, territoryName);
            break;
        }
    }
}


void Player::deployReinforcements() {

    srand(time(0));
    int defendIdx = 0;

    // leave no country undefended at first
    vector<Country*> undefendedTerrs;
    int counter = 0;
    for (Country* country : this->ownedTerritories) {
        // currently undefended
        if (country->getArmiesOnTerritory() == 0) {
            undefendedTerrs.push_back(country);
        }
        counter++;
    }

    vector<Order*> orders;

    // first add reinforcements to any territory that doesn't have armies
    while (defendIdx < undefendedTerrs.size() && this->reinforcementPool > 0) {
        // deploy 5 at a time
        int deployable = (this->reinforcementPool > 5) ? 5 : this->reinforcementPool;
        this->reinforcementPool -= deployable;
        // country is undefended
        // create new deploy order
        Deploy* deployOrder = new Deploy(this, undefendedTerrs[defendIdx], deployable);
        this->ol->addOrder(deployOrder);
        this->addToDeployments(undefendedTerrs[defendIdx]->getCountryId(), deployable);
        defendIdx++;
    }

    defendIdx = 0;
    // add armies by territory
    while (this->reinforcementPool > 0) {
        // deploy 5 at a time
        int deployable = (this->reinforcementPool > 5) ? 5 : this->reinforcementPool;
        this->reinforcementPool -= deployable;
        // country is undefended
        // create new deploy order
        Deploy* deployOrder = new Deploy(this, this->toDefend()[defendIdx], deployable);
        this->ol->addOrder(deployOrder);
        this->addToDeployments(this->ownedTerritories[defendIdx]->getCountryId(), deployable);
        defendIdx = (defendIdx + 1) % this->ownedTerritories.size();
    }
}

void Player::lostCountry(Country* lost) {
    vector<Country*>::iterator it;
    for (it = this->ownedTerritories.begin(); it != this->ownedTerritories.end(); ++it) {
        if ((*it)->getTerritoryName().compare(lost->getTerritoryName()) == 0) {
            break;
        }
    }
    this->ownedTerritories.erase(it);
}



void Player::declareOwner(string countryName) {
    // let the map know whtat the player now owns the territory identified by countryName
    Map* m = worldMap;

    Country* ownedTerritory = worldMap->setPlayerOwnership(this, countryName);

    // HERE: just need to add ownedTerritory to this players list of owned territories
    this->ownedTerritories.push_back(ownedTerritory);
}

queue<Order*> Player::getSortedQueue() {
    return this->ol->getSortedQueue();
}

void Player::clearOrders() {
    this->ol->clear();
}



// sets the map object shared by all players that the player objects can use to declare ownership of territories
void Player::setMap(Map* map) {
    // inside member functions, the players can now access worldMap to do things to territories and stuff
    Player::worldMap = map;
}

void Player::setPlayersInGame(vector<Player*>* playersInGame) {
    Player::playersInGame = playersInGame;
}

void Player::setDeck(Deck* deck) {
    Player::gameDeck = deck;
}

int Player::numOwnedCountries() {
    return this->ownedTerritories.size();
}

int Player::deservesContinentBonus() {
    return Player::worldMap->deservesContinentBonus(this->playerId);
}

void Player::addToReinforcements(int numArmies) {
    this->armies += numArmies;
    this->reinforcementPool += numArmies;
}

OrdersList* Player::getList(){
    return ol;
}


void Player::addCard(Card* card) {
    this->hand->pickCard(card);
}