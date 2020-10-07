class OrderList{
    
    public:
        OrderList();
        void move();
        void Delete();
        void execute();
        bool validate();

};

class Move: public OrderList{

};

class Delete: public OrderList{

};

class Execute: public OrderList{

};



