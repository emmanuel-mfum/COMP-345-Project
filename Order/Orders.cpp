#include <iostream>
#include <string>

#include "Orders.h"

using namespace std;

/*
* 
* Visual Studio is showing me a lot of errors for this file....
* 
* I don't think you need to define the methods for each class. 
* Just define it in the base class and you can use it in the other classes?
* 
* You can't do ClassName.methodName()
* If you want to use a static function, its like ClassName::methodName()
* If you want to use a member method on an instance of a class from within a non-static function
* you can do this->methodName()
* 
*/

void OrderList::Delete(Order or){
    cout<<"Delete object form list"<<endl;
}

void OrderList::move(){
    count<<"Move method"<<endl;
}

Deploy::Deploy(){

}

void Deploy::output(){
    cout<<"Deploy Output method"<<endl;
}

void Deploy::validate(){
    Deploy.validate();
}

void Deploy::execute(){
    Deploy.validate();
}

Advance::Advance(){

}

void Advance::output(){
    cout<<"Advance Output method"<<endl;
}

void Advance::validate(){
    Advance.validate();
}

void Advance::execute(){
    Advance.execute();
}

Bomb::Bomb(){

}

void Bomb::output(){
    cout<<"Bomb Output method"<<endl;
}

void Bomb::validate(){

}

void Bomb::execute(){
    Bomb.validate();
}

Blockade::Blockade(){

}

void Blockade::output(){
    cout<<"Blockade Output method"<<endl;
}

void Blockade::validate(){

}

void Blockade::execute(){
    Blockade::validate();
}

Airlift::Airlift(){

}

void Airlift::output(){
    cout<<"Airlift Output method"<<endl;
}

void Airlift::validate(){

}

void Airlift::execute(){
    Airlift.validate();
}

Negotiate::Negotiate(){

}

void Negotiate::output(){
    cout<<"Negotiate Output method"<<endl;
}

void Negotiate::validate(){

}

void Negotiate::execute(){
    Negotiate.execute();
}






