#pragma once

#include <list>

using namespace std;

class OrderList{
    list<Orders> orderlist;

    public:
    void move();
    void Delete();
    void execute();
    int size;
};

class Orders{

    public:
    void execute();
    void output();
    void validate();
};

class Deploy: public Orders{
    
    public: 
    Deploy();

};

class Advance: virtual public Orders{
   
    public:
    Advance();
};

class Bomb: virtual public Orders{

    public:
    Bomb();
};

class Blockade: virtual public Orders{

    public:
    Blockade();

};

class Airlift: virtual public Orders{

    public:
    Airlift();
};

class Negotiate: virtual public Orders{

    public:
    Negotiate();

};