#include "Orders.h"
#include "Orders.h"

OrdersList::OrdersList(){}

Order::Order(){}

Order::Order(const std::string& type) : type_(type) {
    std::cout <<"A \"" << type_ << "\" order has been created!" << std::endl;
}

Order::~Order(){}

Order::Order(const Order& other) : type_(other.type_) { }

const std::string& Order::getType() const {
    return type_;
}

//Validate method
void Order::validate() {
    std::cout << "The \"" << type_ << "\" order is valid." << std::endl;
}

//Execute method
void Order::execute() {
    std::cout << "A \"" << type_ << "\" order is being executed." << std::endl;
    validate();
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << "Order: " << order.type_ << std::endl;
    return os;
};

//Class Constructor
Deploy::Deploy(){}

Deploy::~Deploy(){}

Deploy::Deploy(const std::string& type) : Order(type) { }

Deploy::Deploy(const Deploy& other) : Order(other.type_) { }

Advance::Advance(){}

Advance::~Advance(){}

Advance::Advance(const std::string& type) : Order(type) { }

Advance::Advance(const Advance& other) : Order(other.type_) { }

Bomb::Bomb(){}

Bomb::~Bomb(){}

Bomb::Bomb(const std::string& type) : Order(type) { }

Bomb::Bomb(const Bomb& other) : Order(other.type_) { }

Blockade::Blockade(){}

Blockade::~Blockade(){}

Blockade::Blockade(const std::string& type) : Order(type) { }

Blockade::Blockade(const Blockade& other) : Order(other.type_) { }

Airlift::Airlift(){}

Airlift::~Airlift(){}

Airlift::Airlift(const std::string& type) : Order(type) { }

Airlift::Airlift(const Airlift& other) : Order(other.type_) { }

Negotiate::Negotiate(){ }

Negotiate::~Negotiate(){}

Negotiate::Negotiate(const std::string& type) : Order(type) { }

Negotiate::Negotiate(const Negotiate& other) : Order(other.type_) {}

OrdersList::~OrdersList(){}

//Add method to add Orders to the list
void OrdersList::addOrder(const Order& order) {
    orders_.push_back(std::make_unique<Order>(order));
    std::cout <<"A \"" <<order.getType() << "\" order has been added to the list!" << std::endl;
}

//Delete method to delete object from list
void OrdersList::Delete(size index) {
    auto iter = orders_.cbegin();           //iterate to find index
    std::advance(iter,index);
    std::cout<<"The \""<< (*iter)->getType()<<"\" at index "<< index<<" has been removed from the list of orders"<<std::endl;
    
    orders_.erase(iter);
}

//Move method
void OrdersList::move(size index1, size index2) {
    auto iter1 = orders_.cbegin();
    auto iter2 = orders_.cbegin();
    std::advance(iter1, index1);
    std::advance(iter2, index2);
    std::cout << "The \""<<(*iter1)->getType() << "\" order at index " << index1
              << " has been moved to index " << index2 << std::endl;
    auto ptr = std::make_unique<Order>(**iter1);
    orders_.insert(iter2, std::move(ptr));
    orders_.erase(iter1);
}

//Execute method
void OrdersList::executeOrders() {
    std::cout<<"Executing all orders in the list"<<std::endl;
    for (auto& i : orders_) {
        i->execute();
    }
}

//Return size of list
unsigned long OrdersList::getsize(){
    return orders_.size();
}


