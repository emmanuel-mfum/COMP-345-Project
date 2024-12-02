#pragma once

#include <vector>
#include <string>
#include <map>
#include "../Map/Map.h"
#include "../Order/Orders.h"
#include "../Card/Cards.h"
#include "../PlayerStrategy/PlayerStrategies.h"
#include <queue>

using namespace std;

class Order;
class OrdersList;
class Map;
class Hand;
class Card;


class Player {
friend class PlayerStrategy;
friend class BenevolentPlayerStrategy;
friend class AggressivePlayerStrategy;
friend class HumanPlayerStrategy;

private:
    // declare a static Map so all players can see it
    static Map* worldMap;
    static vector<Player*>* playersInGame;
    static Deck* gameDeck;

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

    int diplomaticPartner;
    PlayerStrategy* strategy;

public:
    //constructor
    Player();

    Player(int strategyNum);
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
    static void setDeck(Deck* deck);

    void addCard(Card* card);
    void initiateDiplomacy(int diplomacyId) { this->diplomaticPartner = diplomacyId; }
    int getNegotiator() { return this->diplomaticPartner; }
    void endDiplomaticNegotiations() { this->diplomaticPartner = -1; }
};
