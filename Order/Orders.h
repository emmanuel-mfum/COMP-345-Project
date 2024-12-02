
#pragma once

#include <iostream>
#include <list>
#include <iterator>
#include <memory>
#include <vector>
#include "../Player/player.h"
#include "../Map/Map.h"
#include "../Card/Cards.h"
#include <queue>

using namespace std;

class Country;
class Player;
class OrdersList;
class Deck;


class Order {
friend std::ostream& operator<<(std::ostream& os, const Order& order);
public:
    Order();
    ~Order();
    Order(string type, Player* player, Country* country, int priority);
    Order(const Order& other);
    const std::string& getType() const;
    virtual bool validate() = 0;
    virtual void execute() = 0;
    Player* getPlayer();
    Order* nextImpendingAttack; // not sure about that confusing
    Country* getCountry();
    virtual int getPriority() const;
    bool wasSuccess();
    virtual int getNum() = 0;
    virtual string getStringDescription() = 0;

    static void setDeck(Deck* gameDeck);

protected:
    int priority_;
    string type_;
    Player* player;
    Country* country;
    bool failed;

    static Deck* gameDeck;
};



class AttackingOrder : public Order {
protected:
    Country* attacking;
    int numAttackingArmies;
    bool conquered;
    bool wasAttack;
    int survivingAttackers;
    int survivingDefenders;
    int initialDefendingArmies;

    void attack();

public:
    AttackingOrder();
    AttackingOrder(const std::string& type, Player* player, Country* country, int priority, Country* target, int numAttackingArmies);
    AttackingOrder(const AttackingOrder& other);
    ~AttackingOrder();

    virtual bool validate() = 0;
    virtual void execute() = 0;
    int getNum();
    string getStringDescription();
};


std::ostream& operator<<(std::ostream& os, const Order& order);

class Deploy : public Order {
public:
    ~Deploy();
    Deploy();
    Deploy(Player* player, Country* country, int numToDeploy);
    Deploy(const Deploy& other);
    void execute();
    bool validate();

    int getNum() { return this->deploying; }

    string getStringDescription();
private:
    int deploying;
};

class Advance : public AttackingOrder {
public:
    ~Advance();
    Advance();
    Advance(Player* player, Country* country, Country* attacking, int numAttackingArmies);
    Advance(const Advance& other);
    void execute();
    bool validate();
};

class Bomb : public Order {
public:
    ~Bomb();
    Bomb();
    Bomb(Player* player, Country* country);
    Bomb(const Bomb& other);
    void execute();
    bool validate();

    int getNum();
    string getStringDescription();
};

class Blockade : public Order {
public:
    Blockade();
    ~Blockade();
    Blockade(Player* player, Country* country);
    Blockade(const Blockade& other);
    void execute();
    bool validate();

    int getNum();
    string getStringDescription();
};

class Airlift : public AttackingOrder {
public:
    Airlift();
    ~Airlift();
    Airlift(Player* player, Country* country, Country* airliftingTo, int numArmies);
    Airlift(const Airlift& other);
    void execute();
    bool validate();
};

class Negotiate : public Order {
public:
    ~Negotiate();
    Negotiate(Player* player, Country* country, Country* attackingCountry);
    Negotiate(const Negotiate& other);
    void execute();
    bool validate();

    int getNum();
    string getStringDescription();

private:
    Country* attackingCountry;
};

class OrdersList {
public:
    OrdersList();
    ~OrdersList();
    void addOrder(Order* order);
    void Delete(int index);
    void move(int index1, int index2);
    void executeOrders();
    unsigned long getSize();
    void sort();
    vector<Order*> getList();
    queue<Order*> getSortedQueue();
    void clear();


private:
    static bool compare(Order* o1, Order* o2);
    vector<Order*> orders_;
};
