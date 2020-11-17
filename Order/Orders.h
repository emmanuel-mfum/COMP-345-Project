
#pragma once

#include <iostream>
#include <list>
#include <iterator>
#include <memory>

class Country;
class Player;

class Order {
friend std::ostream& operator<<(std::ostream& os, const Order& order);
public:
    Order();
    ~Order();
    Order(const std::string& type);
    Order(const Order& other);
    const std::string& getType() const;
    virtual bool validate() = 0;
    virtual void execute() = 0;
    Player* getPlayer() {};
    Order* nextImpendingAttack; // not sure about that confusing
    //Country* getCountry() {};
protected:
    const std::string type_;
    Player* player;
    Country* country;
};

std::ostream& operator<<(std::ostream& os, const Order& order);

class Deploy : public Order {
public:
    Deploy();
    ~Deploy();
    Deploy(const std::string& type);
    Deploy(const Deploy& other);
    void execute();
    bool validate();
};

class Advance : public Order {
public:
    Advance();
    ~Advance();
    Advance(const std::string& type);
    Advance(const Advance& other);
    void execute();
    bool validate();
};

class Bomb : public Order {
public:
    Bomb();
    ~Bomb();
    Bomb(const std::string& type);
    Bomb(const Bomb& other);
    void execute();
    bool validate();
};

class Blockade : public Order {
public:
    Blockade();
    ~Blockade();
    Blockade(const std::string& type);
    Blockade(const Blockade& other);
    void execute();
    bool validate();
};

class Airlift : public Order {
public:
    Airlift();
    ~Airlift();
    Airlift(const std::string& type);
    Airlift(const Airlift& other);
    void execute();
    bool validate();
};

class Negotiate : public Order {
public:
    Negotiate();
    ~Negotiate();
    Negotiate(const std::string& type);
    Negotiate(const Negotiate& other);
    void execute();
    bool validate();
};

class OrdersList {
typedef std::list<std::unique_ptr<Order>>::size_type size;
public:
    OrdersList();
    ~OrdersList();
    void addOrder(const Order& order);
    void Delete(size index);
    void move(size index1, size index2);
    void executeOrders();
    unsigned long getsize();
private:
    std::list<std::unique_ptr<Order>> orders_;
};
