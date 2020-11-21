#include <vector>

#include "Orders.h"
#include "../Player/player.h"
#include "../Map/Map.h"
#include <iterator>
#include <algorithm>
#include <queue>

using namespace std;

OrdersList::OrdersList(){
    this->orders_;
}

Order::Order() {
    this->country = nullptr;
    this->player = nullptr;
    this->priority_ = -1;
    this->type_ = nullptr;
    this->failed = false;
}

Order::Order(string type, Player* player, Country* country, int priority) {
    this->type_ = type;
    this->player = player;
    this->country = country;
    this->priority_ = priority;
    this->failed = false;
}

Order::~Order(){}

Order::Order(const Order& other) {
    this->type_ = other.type_;
    this->player = other.player;
    this->country = other.country;
    this->priority_ = other.priority_;
    this->failed = other.failed;
}

const std::string& Order::getType() const {
    return type_;
}

int Order::getPriority() const {
    return priority_;
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

AttackingOrder::AttackingOrder() : Order() {
    this->attacking = nullptr;
    this->numAttackingArmies = 0;
    this->conquered = false;
    this->wasAttack = false;
}


AttackingOrder::AttackingOrder(const std::string& type, Player* player, Country* country, int priority, Country* target, int numAttackingArmies) :
    Order(type, player, country, priority) {

    this->attacking = target;
    this->numAttackingArmies = numAttackingArmies;
    this->conquered = false;
    this->wasAttack = false;
}

AttackingOrder::AttackingOrder(const AttackingOrder& other): Order(other) {
    this->attacking = other.attacking;
    this->numAttackingArmies = other.numAttackingArmies;
    this->conquered = other.conquered;
    this->wasAttack = other.wasAttack;
}

AttackingOrder::~AttackingOrder() {}

void AttackingOrder::attack() {
    // TODO CHECK FOR BLOCK!

    this->wasAttack = true;
    int defenders = this->attacking->getArmiesOnTerritory(); // army defending the territory
    int numOfAttackers = this->country->reduceArmies(this->numAttackingArmies); // could be modified to another value
    this->country->resetAdvancing();

    if (defenders != 0) {

        for (int i = 0; i < numOfAttackers; i++) { // for each attacking army
            // every attacker tries to kill a defender
            int randomAssault = rand() % 100; // generate a number between 0 - 100
            if (randomAssault < 60) {
                // defenders are killed
                defenders--;
                if (defenders == 0) {
                    break;
                }
            }
        }

        for (int i = 0; i < this->attacking->getArmiesOnTerritory(); i++) {
            int randomDefence = rand() % 100; // generate a number betweem 0 - 100
            // all attackers have been killed
            if (numOfAttackers == 0) {
                break;
            }
            if (randomDefence < 70) {
                // attackers are killed
                numOfAttackers--;
                
            }
        }

        
    }



    int attackingLossed = this->numAttackingArmies - numOfAttackers;
    int defendingLossed = this->attacking->getArmiesOnTerritory() - defenders;
    this->player->sustainedLosses(attackingLossed);
    cout << "Player " + to_string(this->player->getPlayerId()) + " attacked " + this->attacking->getTerritoryName() + "(" + to_string(this->attacking->getCountryId()) + ") from " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + ")" << endl;
    cout << "   Start: " << to_string(this->numAttackingArmies) + " v " << to_string(this->attacking->getArmiesOnTerritory()) << endl;
    cout << "   Outcome: " + to_string(numOfAttackers) + " v " + to_string(defenders) << endl;

    if (defenders == 0 && numOfAttackers > 0) { 
        // if all the defenders are vanquished
        // attackers conquered
        this->conquered = true;
        this->player->declareOwner(this->attacking->getTerritoryName()); // lets the map know that the player has now another territory
        this->attacking->setArmiesOnTerritory(numOfAttackers);
        /*
        * 
        * TODO DRAW A CARD!
        * 
        * 
        */
    }
    else if (numOfAttackers == 0 && defenders == 0) {
        // stalemate
        // reflect opponent losses
        this->attacking->sustainOpponentLosses(defendingLossed);
        this->attacking->reduceArmies(defendingLossed);
    }
    else if (defenders > 0 && numOfAttackers == 0) {
        // attack repelled
        // reflect opponent losses
        this->attacking->sustainOpponentLosses(defendingLossed);
        this->attacking->reduceArmies(defendingLossed);
    }
    else {
        //this->attacking->sustainOpponentLosses(defendingLossed);
        this->attacking->reduceArmies(defendingLossed);
        this->country->deployArmies(numOfAttackers);
    }
}


//Class Constructor
//Deploy::Deploy(){}
Deploy::Deploy() : Order() {
    this->deploying = 0;
}

Deploy::~Deploy(){}

Deploy::Deploy(Player* player, Country* country, int numToDeploy) : Order("Deploy", player, country, 1) {
    this->deploying = numToDeploy;
}

Deploy::Deploy(const Deploy& other) : Order(other) {
    this->deploying = other.deploying;
}

void  Deploy::execute() {

    if (this->validate()) {
        this->country->deployArmies(this->deploying);
    }
    else {
        this->failed = false;
    }
 
    
}

bool Deploy::validate() {
    return this->player->getPlayerId() == this->country->getPlayerOwnership();
    
}

//Advance::Advance(){}
Advance::Advance() : AttackingOrder() {}

Advance::Advance(Player* player, Country* country, Country* attacking, int numAttackingArmies) : AttackingOrder("Advance",  player, country, 4, attacking, numAttackingArmies) { }

Advance::Advance(const Advance& other) : AttackingOrder(other) { }

Advance::~Advance() {}

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
    
    if (this->validate()) {
         if (this->attacking->getPlayerOwnership() != this->country->getPlayerOwnership()) { // checks if we have to simulate an attack
             this->attack();
             
         }
         else {
             this->country->deployArmies(this->numAttackingArmies);
         }

    }
    else {
        this->failed = true;
    }
}


//Bomb::Bomb(){}
Bomb::Bomb() {}

Bomb::~Bomb(){}

Bomb::Bomb(Player* player, Country* country) : Order("Bomb", player, country, 4) { }

Bomb::Bomb(const Bomb& other) : Order(other) { }

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


Blockade::~Blockade(){}

Blockade::Blockade(Player* player, Country* country) : Order("Blockade", player, country, 3) { }

Blockade::Blockade(const Blockade& other) : Order(other) { }

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
       territoryTarget->setPlayerOwnership(nullptr); // sets the owner's id to the neutral id (kind of , could be modified)

   }

}


Airlift::Airlift(){}

Airlift::~Airlift(){}

Airlift::Airlift(Player* player, Country* country) : Order("Airlift", player, country, 2) { }

Airlift::Airlift(const Airlift& other) : Order(other) { }

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

Negotiate::Negotiate(Player* player, Country* country) : Order("Negotiate" , player, country, 4) { }

Negotiate::Negotiate(const Negotiate& other) : Order(other) {}


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
    
    return valid;

}

void Negotiate::execute() {
    bool validity = this->validate();

    if (validity) {

        int playerID = this->player->getPlayerId();
        vector<Country*> targetTerritories = this->player->toAttack(); // gets all the territories that the player can attack
        int random = rand() % targetTerritories.size(); // generate random number
        int otherPlayerID = targetTerritories[random]->getPlayerOwnership(); // gets id of another player

        // How to prevent attacks between the two players ?!
        Order* incoming = this->nextImpendingAttack; // takes the nextImpendingAttack attribute of the current player
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
void OrdersList::addOrder(Order* order) {
    this->orders_.push_back(order);
}

//Delete method to delete object from list
void OrdersList::Delete(int index) {
    vector<Order*>::iterator it = this->orders_.begin();
    advance(it, index);
    this->orders_.erase(it);
}

//Move method
void OrdersList::move(int index1, int index2) {
    vector<Order*>::iterator it = this->orders_.begin();
    Order* moving = this->orders_[index1];
    // remove it from index1
    advance(it, index1);
    this->orders_.erase(it);
    // move it to index2
    it = this->orders_.begin();
    advance(it, index2);
    this->orders_.insert(it, moving);
}

//Execute method
void OrdersList::executeOrders() {
    std::cout<<"Executing all orders in the list"<<std::endl;
    for (auto& i : orders_) {
        i->execute();
    }
}

//Return size of list
unsigned long OrdersList::getSize(){
    return this->orders_.size();
}

bool OrdersList::compare(Order* o1, Order* o2) {
    std::cout << o2->getPriority() << std::endl;
    return o1->getPriority() < o2->getPriority();
}

void OrdersList::sort() {
    std::sort(orders_.begin(), orders_.end(), OrdersList::compare);
}

vector<Order*> OrdersList::getList() {
    return orders_;
}


queue<Order*> OrdersList::getSortedQueue() {
    this->sort();
    queue<Order*> sortedOrders;
    for (Order* o : this->orders_) {
        sortedOrders.push(o);
    }
    return sortedOrders;
}

void OrdersList::clear() {
    this->orders_.clear();
}