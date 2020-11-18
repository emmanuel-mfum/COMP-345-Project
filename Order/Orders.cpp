#include "Orders.h"
#include "Orders.h"
#include "../Player/player.h"
#include "../Map/Map.h"

OrdersList::OrdersList(){}

Order::Order(){}

Order::Order(const std::string& type, Player* player, Country* country) : type_(type), player(player), country(country) {
    std::cout <<"A \"" << type_ << "\" order has been created!" << std::endl;
}

Order::~Order(){}

Order::Order(const Order& other) : type_(other.type_) { }

const std::string& Order::getType() const {
    return type_;
}

//Validate method
//void Order::validate() {
    //std::cout << "The \"" << type_ << "\" order is valid." << std::endl;
//}

//Execute method
//void Order::execute() {
   // std::cout << "A \"" << type_ << "\" order is being executed." << std::endl;
   // validate();
//}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << "Order: " << order.type_ << std::endl;
    return os;
};

Player* Order::getPlayer() {
    return this->player;
};

Country* Order::getCountry() {
    return this->country;
}

//Class Constructor
//Deploy::Deploy(){}

Deploy::Deploy() :Order() {} // default constructor

Deploy::~Deploy(){}

Deploy::Deploy(const std::string& type , Player* player, Country* country) : Order(type, player, country) { }

Deploy::Deploy(const Deploy& other) : Order(other.type_) { }

void  Deploy::execute() {
    bool validity = this->validate();

    if (validity == true) {
        
        vector<Country*> currentTerritories = this->player->toDefend(); // gets all the territories owned by the player
        int random = rand() % currentTerritories.size(); // generate a random number between 0 and the number of territories owned by the player
        Country* destination = currentTerritories[random]; // assign pointer to a random territory


        int id = this->player->getPlayerId(); // gets the id of the current player
        int reserve = this->player->getReinforcementPool(); // gets the number of armies in the reinforment pool
        int reinforcement =  reserve; // calculates number of reinforcement
        
        destination->setArmiesOnTerritory(reinforcement + destination->getArmiesOnTerritory());
        //currentPlayer->setReinforcementPool(reserve-reinforcement) not sure if necessary
    }
    else {
        cout << "Order was invalid " << endl;
    }
 
    
}

bool Deploy::validate()
{
    int id = this->player->getPlayerId(); // gets the id of the 
    //Country* currentTerritory = this->country;
    vector<Country*> currentTerritories = this->player->toDefend(); // gets all the territories owned by the player
    bool owned; // boolean "counter"

    for (int i = 0; i < currentTerritories.size(); i++) { // loops across all the territories owned by the player

        if (id == currentTerritories[i]->getPlayerOwnership()) { // checks if the ids of the player and the of all territories owned by him are the same
            owned = true; 
        } 
        else {  // else a territory is not owned by the player
            owned = false;
            break;
        }


    }

    return owned; // return boolean variable
    
}

//Advance::Advance(){}

Advance::Advance(): Order() {} // default constructor

Advance::~Advance(){}

Advance::Advance(const std::string& type, Player* player, Country* country) : Order(type, player, country) { }

Advance::Advance(const Advance& other) : Order(other.type_) { }

bool Advance::validate() {

    int id = this->player->getPlayerId(); // gets the player's id 
    int owner = this->country->getPlayerOwnership(); // check the id of the source territory
   

    if (id == owner) { // if both id are the same return true
        return true;
    }

    else { // else return false
        return false;
    }
}

void Advance::execute() {
    bool validity = this->validate();
    Country* sourceTerritory = this->country;
    int movingArmies = sourceTerritory->getArmiesOnTerritory(); // gets army on the source territory
    //int totalArmies = currentPlayer->getArmies();
    int totalArmies = movingArmies; // armies on source territory
    //currentPlayer->nextImpendingAttack = this; // try to use Paul's advice here
    if (validity) {
        vector<Country*> adjacent; // pointer to adjacent territories
        //Country* currentTerritory = this->country; // current territory inside the Advance object
        // adjacent = currentTerritory->getAdjacentCountries(); // points to territories adjacent to current territory
        adjacent = this->player->toAttack(); // gets a vector of potential territories to attack

         int random = rand() % adjacent.size(); // generate a random number

         Country* nearby = adjacent[random]; // takes random adjacent territory

         

         int ownership = nearby->getPlayerOwnership();
         int id = this->player->getPlayerId();

         if (ownership != id) { // checks if we have to simulate an attack

             nextImpendingAttack = this; //

             int defenders = nearby->getArmiesOnTerritory(); // army defending the territory
             int survivingDefenders = 0;
             int numOfAttackers = totalArmies; // could be modified to another value 

             for (int i = 0; i < numOfAttackers; i++) { // for each attacking army
                 // every attacker tries to kill a defender
                  int randomAssault = rand() % 100 ; // generate a number between 0 - 100
                  int randomDefence = rand() % 100; // generate a number betweem 0 - 100
                  if (randomAssault < 60) {
                      // defenders are killed
                      defenders--;
                  }

                  if (randomDefence < 70) {
                      // attackers are killed
                      numOfAttackers--;
                  }

                
             }

             if (defenders == 0) { // if all the defenders are vainquished
                 string conquest = nearby->getTerritoryName();
                 this->player->declareOwner(conquest); // lets the map know that the player has now another territory
                 nearby->setArmiesOnTerritory(1); // bring armies on that territory, here I just move one army
                 totalArmies--; // decrement the number of armies of the player
                 this->player->setArmies(totalArmies); // sets it to a new value for the current Player

                 // Moreover player should get a Card, but how ? Does the Player has a vector of Cards somewhere
             }

             

             nextImpendingAttack;
         }
         else {

             nearby->setArmiesOnTerritory(1); // bring armies on that territory, here I just move one army
             totalArmies--; // decrement the number of armies of the player
             this->player->setArmies(totalArmies); // sets it to a new value for the current Player

         }

    }
    else {
        cout << "Order is invalid " << endl;
    }
}


//Bomb::Bomb(){}

Bomb::Bomb():Order() {} // default constructor

Bomb::~Bomb(){}

Bomb::Bomb(const std::string& type, Player* player, Country* country) : Order(type, player, country) { }

Bomb::Bomb(const Bomb& other) : Order(other.type_) { }

bool Bomb::validate() {
    vector <Country*>adjacent = this->player->toAttack(); // gets a vector of potential territories to attack
    int playerID = this->player->getPlayerId(); // takes the player id
    bool target = true;

    for (int i = 0; i < adjacent.size(); i++) {

        if (playerID == adjacent[i]->getPlayerOwnership()) { // compares the player id with the id associated with every potential target territory

            target = false; // // if one territory doesn't belong to the player, set variable to false
            break;//  exit the for loop
        }

    }

    return target;


}

void Bomb::execute() {
    bool validity = this->validate(); // checks if order is valid 
    if (validity) { // this implies that the territory belongs to the enemy

        vector <Country*>adjacent = this->player->toAttack(); // gets a vector of potential territories to attack
        int random = rand() % adjacent.size(); // generate a random number between 0 and the size of the vector
        Country* territoryTarget = adjacent[random];

         int victims = territoryTarget->getArmiesOnTerritory(); // number of armies on target territory

         int survivors = victims / 2; // number of armies halved by the bombing

         territoryTarget->setArmiesOnTerritory(survivors); // set the new number of armies on the bombed territory


    }
    else {
        cout << "The order is invalid " << endl;
    }


}


//Blockade::Blockade(){}

Blockade::Blockade():Order() {} // default constructor

Blockade::~Blockade(){}

Blockade::Blockade(const std::string& type, Player* player, Country* country) : Order(type, player, country) { }

Blockade::Blockade(const Blockade& other) : Order(other.type_) { }

bool Blockade::validate() {

    vector <Country*>adjacent = this->player->toDefend(); // gets a vector of potential territories to defend (his own)
    int playerID = this->player->getPlayerId(); // takes the player id
    bool target;

    for (int i = 0; i < adjacent.size(); i++) {

        if (playerID != adjacent[i]->getPlayerOwnership()) { // compares the player id with the id associated with every potential target territory

            target = false; // if one territory doesn't belong to the player, set variable to false
            break; // exit the for loop
        }

    }

    return target;


}

void Blockade::execute() {
   bool validity =  this->validate(); // checks validity

   if (validity) {

       vector <Country*>adjacent = this->player->toDefend(); // gets a vector of potential territories to defend
       int random = rand() % adjacent.size(); // generate a random number between 0 and the size of the vector
       Country* territoryTarget = adjacent[random]; // gets a random territory 

       int intialArmy = territoryTarget->getArmiesOnTerritory(); // gets the number of armies already present on the territory
       int doubleArmy = intialArmy * 2; // doubles the army size

       territoryTarget->setArmiesOnTerritory(doubleArmy); // set the number of armies to double the initial number
       int neutralID = -1 ;
       territoryTarget->setPlayerOwnership(neutralID); // sets the owner's id to the neutral id (kind of , could be modified)

   }

}

//Airlift::Airlift(){}

Airlift::Airlift():Order() {}

Airlift::~Airlift(){}

Airlift::Airlift(const std::string& type, Player* player, Country* country) : Order(type, player, country) { }

Airlift::Airlift(const Airlift& other) : Order(other.type_) { }

bool Airlift::validate() {

    bool target;

    int id = this->player->getPlayerId(); // gets id of the current player
   

    if (this->country->getPlayerOwnership() == this->player->getPlayerId()) { // if the source territory is  owned by the player
            target = true;
     }
    else {
          target = false;
     }
  

    return target;


}

void Airlift::execute() {
    bool validity = this->validate(); //checks validity


    if (validity) {
        vector <Country*> targetTerritories = this->player->toDefend(); // makes a vector pointing at all territories owned by the player
        //currentPlayer->nextImpendingAttack = this; // try to use Paul's advice here
        int currentArmies = this->player->getArmies();
        int attackingArmies = currentArmies -1 ; // random calculation
        /*
        *Not sure about this part, the PDF says to look at the advance order for the move of armies
        *
        */

        int random = rand() % targetTerritories.size(); // generate a random number

        Country* nearby = targetTerritories[random]; // takes random adjacent territory

        int ownership = nearby->getPlayerOwnership();
        int id = this->player->getPlayerId();

        if (ownership != id) { // checks if we have to simulate an attack

            int defenders = nearby->getArmiesOnTerritory(); // army defending the territory
            int survivingDefenders = 0;
            int numOfAttackers = attackingArmies; // could be modified to another value 

            for (int i = 0; i < numOfAttackers; i++) { // for each attacking army
                // every attacker tries to kill a defender
                int randomAssault = rand() % 100; // generate a number between 0 - 100
                int randomDefence = rand() % 100; // generate a number betweem 0 - 100
                if (randomAssault < 60) {
                    // defenders are killed
                    defenders--;
                }

                if (randomDefence < 70) {
                    // attackers are killed
                    numOfAttackers--;
                }


            }

            if (defenders == 0) { // if all the defenders are vainquished
                string conquest = nearby->getTerritoryName();
                this->player->declareOwner(conquest); // lets the map know that the player has now another territory
                nearby->setArmiesOnTerritory(1); // bring armies on that territory, here I just move one army
                //totalArmies--; // decrement the number of armies of the player
                this->player->setArmies(numOfAttackers); // sets it to a new value for the current Player

                // Moreover player should get a Card, but how ? Does the Player has a vector of Cards somewhere
            }


            this->nextImpendingAttack = nullptr; // Here it means the attack is done and finished

        }
        else {

            nearby->setArmiesOnTerritory(1); // bring armies on that territory, here I just move one army
            currentArmies--; // decrement the number of armies of the player
            this->player->setArmies(currentArmies); // sets it to a new value for the current Player

        }
    }

    else {
        cout << "Order is invalid" << endl;
    }


}

//Negotiate::Negotiate(){ }

Negotiate::~Negotiate(){}

Negotiate::Negotiate(const std::string& type, Player* player, Country* country) : Order(type , player, country) { }

Negotiate::Negotiate(const Negotiate& other) : Order(other.type_) {}

Negotiate::Negotiate():Order(){}

Negotiate::~Negotiate(){}

Negotiate::Negotiate(const std::string& type) : Order(type) { }

Negotiate::Negotiate(const Negotiate& other) : Order(other.type_) {}

bool Negotiate::validate() {
    bool valid;
   
    int playerID = this->player->getPlayerId();
    vector<Country*> targetTerritories = this->player->toAttack(); // gets all the territories that the player can attack

    for (int i = 0; i < targetTerritories.size(); i++) {
        int otherPlayerID = targetTerritories[i]->getPlayerOwnership();

        if (playerID != otherPlayerID) { // checks if the ids are different
            valid = true;
        }
        else { // if for one territory the owner is the same as the player the order is invalid
            valid = false;
            break;
        }
    }
   

}

void Negotiate::execute() {
    bool validity = this->validate();

    if (validity) {

        int playerID = this->player->getPlayerId();
        vector<Country*> targetTerritories = this->player->toAttack(); // gets all the territories that the player can attack
        int random = rand() % targetTerritories.size(); // generate random number
        int otherPlayerID = targetTerritories[random]->getPlayerOwnership(); // gets id of another player

        // How to prevent attacks between the two players ?!
        Order* incoming = this->player->nextImpendingAttack; // takes the nextImpendingAttack attribute of the current player
        Player* sourceAttack = incoming->getPlayer(); //  gets the player that is the source of that attack
        int playerIdBeingAttacked = sourceAttack->getPlayerId(); // get the id that is p

        if ( playerIdBeingAttacked == this->player->getPlayerId()) { // the player being targeted by the attack is the same as the player playing the negotiate

        }



    }
    else {
        cout << " The order is invalid" << endl;
    }


}

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


