
#pragma once

#include <iostream>
#include <list>
#include <iterator>
#include <memory>
#include <vector>

using namespace std;

class Country;
class Player;

class Order {
friend std::ostream& operator<<(std::ostream& os, const Order& order);
public:
    Order();
    ~Order();
    Order(const std::string& type, Player* player, Country* country, int priority);
    Order(const Order& other);
    const std::string& getType() const;
    virtual bool validate() = 0;
    virtual void execute() = 0;
    Player* getPlayer();
    Order* nextImpendingAttack; // not sure about that confusing
    Country* getCountry();
    virtual int getPriority() const;
protected:
    const int priority_ = 0;
    const std::string type_;
    Player* player;
    Country* country;
};

std::ostream& operator<<(std::ostream& os, const Order& order);

class Deploy : public Order {
public:
    Deploy();
    ~Deploy();
    Deploy(const std::string& type, Player* player, Country* country, int priority);
    Deploy(const Deploy& other);
    void execute();
    bool validate();
};

class Advance : public Order {
public:
    Advance();
    ~Advance();
    Advance(const std::string& type, Player* player, Country* country, int priority);
    Advance(const Advance& other);
    void execute();
    bool validate();
};

class Bomb : public Order {
public:
    Bomb();
    ~Bomb();
    Bomb(const std::string& type, Player* player, Country* country, int priority);
    Bomb(const Bomb& other);
    void execute();
    bool validate();
};

class Blockade : public Order {
public:
    Blockade();
    ~Blockade();
    Blockade(const std::string& type, Player* player, Country* country, int priority);
    Blockade(const Blockade& other);
    void execute();
    bool validate();
};

class Airlift : public Order {
public:
    Airlift();
    ~Airlift();
    Airlift(const std::string& type, Player* player, Country* country, int priority);
    Airlift(const Airlift& other);
    void execute();
    bool validate();
};

class Negotiate : public Order {
public:
    Negotiate();
    ~Negotiate();
    Negotiate(const std::string& type, Player* player, Country* country, int priority);
    Negotiate(const Negotiate& other);
    void execute();
    bool validate();
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

private:
    static bool compare(Order* o1, Order* o2);
    vector<Order*> orders_;
};
