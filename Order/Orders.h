#ifndef Orders_h
#define Orders_h

#include <iostream>
#include <list>
#include <iterator>
#include <memory>

class Order {
friend std::ostream& operator<<(std::ostream& os, const Order& order);
public:
    Order();
    ~Order();
    Order(const std::string& type);
    Order(const Order& other);
    const std::string& getType() const;
    virtual void validate();
    virtual void execute();
protected:
    const std::string type_;
};

std::ostream& operator<<(std::ostream& os, const Order& order);

class Deploy : public Order {
public:
    Deploy();
    ~Deploy();
    Deploy(const std::string& type);
    Deploy(const Deploy& other);
};

class Advance : public Order {
public:
    Advance();
    ~Advance();
    Advance(const std::string& type);
    Advance(const Advance& other);
};

class Bomb : public Order {
public:
    Bomb();
    ~Bomb();
    Bomb(const std::string& type);
    Bomb(const Bomb& other);
};

class Blockade : public Order {
public:
    Blockade();
    ~Blockade();
    Blockade(const std::string& type);
    Blockade(const Blockade& other);
};

class Airlift : public Order {
public:
    Airlift();
    ~Airlift();
    Airlift(const std::string& type);
    Airlift(const Airlift& other);
};

class Negotiate : public Order {
public:
    Negotiate();
    ~Negotiate();
    Negotiate(const std::string& type);
    Negotiate(const Negotiate& other);
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

#endif

