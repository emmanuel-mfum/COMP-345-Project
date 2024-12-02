#include <vector>

#include "Orders.h"
#include "../Player/player.h"
#include "../Map/Map.h"
#include "../Card/Cards.h"
#include <iterator>
#include <algorithm>
#include <queue>

using namespace std;


Deck* Order::gameDeck = nullptr;


OrdersList::OrdersList(){
    this->orders_;
}

void Order::setDeck(Deck* gameDeck) {
    Order::gameDeck = gameDeck;
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

bool Order::wasSuccess() {
    return !this->failed;
}

AttackingOrder::AttackingOrder() : Order() {
    this->attacking = nullptr;
    this->numAttackingArmies = 0;
    this->conquered = false;
    this->wasAttack = false;
    this->survivingAttackers = 0;
    this->survivingDefenders = 0;
    this->initialDefendingArmies = 0;
}


AttackingOrder::AttackingOrder(const std::string& type, Player* player, Country* country, int priority, Country* target, int numAttackingArmies) :
    Order(type, player, country, priority) {

    this->attacking = target;
    this->numAttackingArmies = numAttackingArmies;
    this->conquered = false;
    this->wasAttack = false;
    this->survivingAttackers = 0;
    this->survivingDefenders = 0;
    this->initialDefendingArmies = 0;
}

AttackingOrder::AttackingOrder(const AttackingOrder& other): Order(other) {
    this->attacking = other.attacking;
    this->numAttackingArmies = other.numAttackingArmies;
    this->conquered = other.conquered;
    this->wasAttack = other.wasAttack;
    this->survivingAttackers = other.survivingAttackers;
    this->survivingDefenders = other.survivingDefenders;
    this->initialDefendingArmies = other.initialDefendingArmies;
}

AttackingOrder::~AttackingOrder() {}

void AttackingOrder::attack() {
    // TODO CHECK FOR BLOCK!
    if (this->attacking->getPlayerOwnership() != -1 && this->attacking->diplomaticallyBlocked(this->player->getPlayerId())) {
        this->failed = true;
        return;
    }

    this->wasAttack = true;
    int defenders = this->attacking->getArmiesOnTerritory(); // army defending the territory
    int numOfAttackers = this->country->reduceArmies(this->numAttackingArmies); // could be modified to another value
    this->country->resetAdvancing();
    this->initialDefendingArmies = defenders;

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
    this->survivingAttackers = numOfAttackers;
    this->survivingDefenders = defenders;

    if (defenders == 0 && numOfAttackers > 0) { 
        // if all the defenders are vanquished
        // attackers conquered
        this->conquered = true;
        this->player->declareOwner(this->attacking->getTerritoryName()); // lets the map know that the player has now another territory
        this->attacking->setArmiesOnTerritory(numOfAttackers);
        
        // DRAW A CARD
        Card* card = Order::gameDeck->draw();
        if (card != nullptr) {
            this->player->addCard(card);
        }
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

int AttackingOrder::getNum() {
    return this->numAttackingArmies;
}

string AttackingOrder::getStringDescription() {
    string returning;
    if (this->failed) {
        returning = "      " + this->getType() + " - Player " + to_string(this->player->getPlayerId()) + "'s attack on " + this->attacking->getTerritoryName() + "(" + to_string(this->attacking->getCountryId()) + ") from " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + ") FAILED";
    }
    else if (this->wasAttack) {
        returning = "      " + this->getType() + " - Player " + to_string(this->player->getPlayerId()) + " attacked " + this->attacking->getTerritoryName() + "(" + to_string(this->attacking->getCountryId()) + ") from " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + ")" + "\n";
        returning += "         Start: " + to_string(this->numAttackingArmies) + " v " + to_string(this->initialDefendingArmies) + "\n";
        returning += "         Outcome: " + to_string(this->survivingAttackers) + " v " + to_string(this->survivingDefenders);
        if (this->conquered) {
            returning += "\n         CONQUERED!";
        }
    }
    else {
        returning = "      " + this->getType() + " - Player " + to_string(this->player->getPlayerId()) + " deployed " + to_string(this->numAttackingArmies) + " to " + this->attacking->getTerritoryName() + "(" + to_string(this->attacking->getCountryId()) + ") from " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + ")";
    }
    return returning;
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
        this->failed = true;
    }    
}

bool Deploy::validate() {
    return this->player->getPlayerId() == this->country->getPlayerOwnership();
    
}

string Deploy::getStringDescription() {
    return "      Deploy - Player " + to_string(this->player->getPlayerId()) + " deployed " + to_string(this->deploying) + " to " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + ")";
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
             this->attacking->deployArmies(this->numAttackingArmies);
             this->country->reduceArmies(this->numAttackingArmies);
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
    return this->player->getPlayerId() != this->country->getPlayerOwnership();


}

void Bomb::execute() {
    bool validity = this->validate(); // checks if order is valid 
    if (validity) { // this implies that the territory belongs to the ene
        this->country->bomb();

    }
    else {
        this->failed = true;
    }
}

int Bomb::getNum() {
    return this->country->getPlayerOwnership();
}

string Bomb::getStringDescription() {
    if (!this->failed) {
        return "      BOMB - Player " + to_string(this->player->getPlayerId()) + " bombed " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + "), reducing number of armies to " + to_string(this->country->getArmiesOnTerritory());
    }
    else {
        return "      BOMB - Player " + to_string(this->player->getPlayerId()) + " ordering bombing of " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + "), but it was aborted";
    }
}


//Blockade::Blockade(){}


Blockade::~Blockade(){}

Blockade::Blockade(Player* player, Country* country) : Order("Blockade", player, country, 3) { }

Blockade::Blockade(const Blockade& other) : Order(other) { }

bool Blockade::validate() {

    return this->player->getPlayerId() == this->country->getPlayerOwnership();
}



void Blockade::execute() {
   bool validity =  this->validate(); // checks validity

   if (validity) {
       this->country->blockade();
   }
   else {
       this->failed = true;
   }

}

int Blockade::getNum() {
    return this->country->getArmiesOnTerritory();
}

string Blockade::getStringDescription() {
    if (!this->failed) {
        return "      Blockade - Player " + to_string(this->player->getPlayerId()) + " blockaded " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + "), making it neutral and increasing number of armies to " + to_string(this->country->getArmiesOnTerritory());
    }
    else {
        return "      Blockade - Player " + to_string(this->player->getPlayerId()) + " attempted to blockade " + this->country->getTerritoryName() + "(" + to_string(this->country->getCountryId()) + "), but the attempt failed";
    }
}


Airlift::Airlift() : AttackingOrder() {}

Airlift::~Airlift(){}

Airlift::Airlift(Player* player, Country* country, Country* airliftingTo, int numArmies) : AttackingOrder("Airlift", player, country, 2, airliftingTo, numArmies) { }

Airlift::Airlift(const Airlift& other) : AttackingOrder(other) { }

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


    if (this->validate()) {
        if (this->attacking->getPlayerOwnership() != this->country->getPlayerOwnership()) { // checks if we have to simulate an attack
            this->attack();

        }
        else {
            this->attacking->deployArmies(this->numAttackingArmies);
            this->country->reduceArmies(this->numAttackingArmies);
        }

    }
    else {
        this->failed = true;
    }


}

//Negotiate::Negotiate(){ }

Negotiate::~Negotiate(){}

Negotiate::Negotiate(Player* player, Country* country, Country* attackingCountry) : Order("Negotiate" , player, country, 4) {

    this->attackingCountry = attackingCountry;
}

Negotiate::Negotiate(const Negotiate& other) : Order(other) {}


bool Negotiate::validate() {
    return this->player->getPlayerId() != this->attackingCountry->getPlayerOwnership();
}

void Negotiate::execute() {
    bool validity = this->validate();

    if (validity) {
        this->attackingCountry->initiateDiplomacy(player->getPlayerId());
        this->player->initiateDiplomacy(this->attackingCountry->getPlayerOwnership());
    }
    else {
        this->failed = true;
    }


}

int Negotiate::getNum() {
    return this->attackingCountry->getPlayerOwnership();
}

string Negotiate::getStringDescription() {
    if (!this->failed) {
        return "      Negotiate - Player " + to_string(this->player->getPlayerId()) + " initiated diplomacy with Player " + to_string(this->attackingCountry->getPlayerOwnership()) + ", preventing attacks between them for the rest of the round";
    }
    else {
        return "      Negotiate - Player " + to_string(this->player->getPlayerId()) + " attempted to initiate diplomacy with Player " + to_string(this->attackingCountry->getPlayerOwnership()) + ", but the attempt failed";
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