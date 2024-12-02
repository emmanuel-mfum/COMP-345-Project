#include "Cards.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <ctime>

#include "../Order/Orders.h"
#include "../Player/player.h"
#include "../Map/Map.h"

using namespace std;

const int number_of_cards = 50; // number of cards in a deck

class Order;
class Bomb;
class Deploy;
class Blockade;
class Airlift;
class Negotiate;
class Player;
class Country;




Player* Card::currentlyPlayer = nullptr;
Country* Card::currentlyAttacking = nullptr;
Country* Card::currentSource = nullptr;
int Card::numArmies = 0;


void Card::setCurrentInfo(Player* player, Country* attacking, Country* currentSource, int numArmies) {
	Card::currentlyPlayer = player;
	Card::currentlyAttacking = attacking;
	Card::currentSource = currentSource;
	Card::numArmies = numArmies;
}

void Card::unsetCurrentInfo() {
	Card::currentlyPlayer = nullptr;
	Card::currentlyAttacking = nullptr;
	Card::currentSource = nullptr;
	Card::numArmies = 0;
}



Card::Card() { //default constructor

}

Card::Card(string t) {
	 this->type = t; // set the type at creation
}

Card::~Card(){} // destructor

Card::Card(const Card& obj) { // copy constructor
	this->type = obj.type;
	

}

Card& Card :: operator =(const Card& c) // assignment operator overload
{
	this->type = c.type;

	return *this;
}

	
void Card:: play() { // play method

	cout << "The card has been played" << endl;

}

string Card::getType() {
	return type; // returns the type
}

string Card::print() const {
	return ("The type of the card is " + type); // returns the type of the card
}



Deck::Deck() {
	string types[] = { "BombCard","Reinforcement","BlockadeCard","AirliftCard","Diplomacy" }; // the 5 possible types of cards
	

	for (int i = 0; i < 10; i++) {
		cards.push_back(new BombCard(types[0]));
		cards.push_back(new Reinforcement(types[1]));
		cards.push_back(new BlockadeCard(types[2]));
		cards.push_back(new AirliftCard(types[3]));
		cards.push_back(new Diplomacy(types[4]));
	}
	
}

Deck::Deck(const Deck& obj) { // copy constructor
	this->cards = obj.cards;


}

Deck::~Deck(){// destructor

	for (int i = 0; i < cards.size(); i++) {

		if (cards.at(i) != nullptr) {

			delete cards.at(i);
			cards.at(i) = nullptr;
		}
	}


} 

void Deck::shuffleDeck() { // randomizes the deck


	for (int i = 0; i < cards.size(); i++) {

		int random = (rand() + time(0))%cards.size();// generates a random number between 0 and the size of the deck

		Card* temp = cards[i]; // store the object in temp variable
		cards[i] = cards.at(random); //picks an object at a random position in the deck and stores it inside the current position (index)
		cards[random] = temp; // stores the object in the temp into the position selected at random.
	}

	cout << "The deck has been shuffled for the players" << endl;
}

Card* Deck::draw() // draws a card from the deck
{	
	if (this->cards.size() > 0) {
		Card* a = cards[0];// stores the card from the back of the deck
		cards.erase(cards.begin() + 0); // remove the last card (the one just taken
		return a;
	}
	else {
		return nullptr;
	}
}

void Deck::printDeck() // prints all the cards in the deck
{
	cout << left;

	for (int i = 0; i < cards.size(); i++) {
		cout << this->cards.at(i)->print() << endl;      
	}
}

void Deck::backToDeck(Card* c) { // takes a card and inserts it at the start of the deck

	cards.insert(cards.begin(), c);
}

void Deck::returnToDeck(Card* c) {
	this->cards.push_back(c);
}

Hand::Hand() { // default constrcutor
	this->cards;
};


Hand::Hand(const Hand& obj) { // copy constructor
	this->cards = obj.cards;


}
Hand::~Hand(){} // destructor
	

void Hand::pickCard(Card* c) { // takes a Card object and puts it in the hand
	cards.push_back(c);
}

vector<Card*>Hand::getCards() { // return the vector of pointers to Cards
	return cards;
}


 Card* Hand::removeFromHand(int index) { // remove a Card object from the Hand at a specified index
	int i = index;
	Card* b = cards.at(i);//b is a deep copy of Cards at i
	//delete (check the slides about deep copy operator overload)
	cards.erase(cards.begin() + i);
	cout << "Card removed from hand !" << endl;
	return b;
}

 Card* Hand::getCard() {
	 Card* card = this->cards[0];
	 cards.erase(cards.begin() + 0);
	 return card;
}


 string Hand::printHand() // prints all the cards in the hand
 {
	 cout << left;

	 for (int i = 0; i < cards.size(); i++) {
		 return this->cards.at(i)->print();
	 }
 }


BombCard::BombCard():Card() { //default constructor

 }

 BombCard::BombCard(string t):Card(t) {
	 //this->type = t; // set the type at creation
 }

 BombCard::~BombCard() {} // destructor

 BombCard::BombCard(const BombCard& obj):Card(obj) { // copy constructor
	 //this->type = obj.type;


 }

 BombCard& BombCard :: operator =(const BombCard& c) // assignment operator overload
 {
	 Card::operator =(c);
	 //this->type = c.type;

	 return *this;
 }

 Order* BombCard::getOrder() {
	 return new Bomb(Card::currentlyPlayer, Card::currentlyAttacking);
 }

 Reinforcement::Reinforcement() :Card() { //default constructor

 }

 Reinforcement::Reinforcement(string t) : Card(t) {
	 //this->type = t; // set the type at creation
 }

 Reinforcement::~Reinforcement() {} // destructor

 Reinforcement::Reinforcement(const Reinforcement& obj) :Card(obj) { // copy constructor
	 //this->type = obj.type;


 }

 Reinforcement& Reinforcement :: operator =(const Reinforcement& c) // assignment operator overload
 {
	 Card::operator =(c);
	 //this->type = c.type;

	 return *this;
 }

 Order* Reinforcement::getOrder() {
	 return new Deploy(Card::currentlyPlayer, Card::currentSource, rand() % 100);
 }


 BlockadeCard::BlockadeCard() :Card() { //default constructor

 }

 BlockadeCard::BlockadeCard(string t) : Card(t) {
	 //this->type = t; // set the type at creation
 }

 BlockadeCard::~BlockadeCard() {} // destructor

 BlockadeCard::BlockadeCard(const BlockadeCard& obj) :Card(obj) { // copy constructor
	 //this->type = obj.type;


 }

 BlockadeCard& BlockadeCard :: operator =(const BlockadeCard& c) // assignment operator overload
 {
	 Card::operator =(c);
	 //this->type = c.type;

	 return *this;
 }

 Order* BlockadeCard::getOrder() {
	 return new Blockade(Card::currentlyPlayer, Card::currentSource);
 }

 AirliftCard::AirliftCard() :Card() { //default constructor

 }

 AirliftCard::AirliftCard(string t) : Card(t) {
	 //this->type = t; // set the type at creation
 }

 AirliftCard::~AirliftCard() {} // destructor

 AirliftCard::AirliftCard(const AirliftCard& obj) :Card(obj) { // copy constructor
	 //this->type = obj.type;


 }

 AirliftCard& AirliftCard :: operator =(const AirliftCard& c) // assignment operator overload
 {
	 Card::operator =(c);
	 //this->type = c.type;

	 return *this;
 }

 Order* AirliftCard::getOrder() {
	 return new Airlift(Card::currentlyPlayer, Card::currentSource, Card::currentlyAttacking, Card::numArmies);
 }

 Diplomacy::Diplomacy() :Card() { //default constructor

 }

 Diplomacy::Diplomacy(string t) : Card(t) {
	 //this->type = t; // set the type at creation
 }

 Diplomacy::~Diplomacy() {} // destructor

 Diplomacy::Diplomacy(const Diplomacy& obj) :Card(obj) { // copy constructor
	 //this->type = obj.type;


 }

 Diplomacy& Diplomacy :: operator =(const Diplomacy& c) // assignment operator overload
 {
	 Card::operator =(c);
	 //this->type = c.type;

	 return *this;
 }

 Order* Diplomacy::getOrder() {
	 return new Negotiate(Card::currentlyPlayer, Card::currentSource, Card::currentlyAttacking);
 }


