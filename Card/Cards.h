#pragma once
#include <string>
#include <vector>
#include "../Order/Orders.h"
#include "../Player/player.h"
#include "../Map/Map.h"


using namespace std;



class Order;
class Bomb;
class Deploy;
class Blockade;
class Airlift;
class Negotiate;
class Player;
class Country;

class Card {
public: 
	Card();// default constructor
	Card(string t); // parameterized constructor
	Card(const Card& c); // copy constructor
	~Card(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const Card& toOut); // stream operator overload
	Card& operator =(const Card& c); // assignment operator overload
	void play(); 
	string print() const;
	string getType();
	virtual Order* getOrder() = 0;

	static void setCurrentInfo(Player* player, Country* attacking, Country* currentSource, int numArmies);
	static void unsetCurrentInfo();

private:
	string type;

protected:
	static Player* currentlyPlayer;
	static Country* currentlyAttacking;
	static Country* currentSource;
	static int numArmies;
};


class Deck {
public:
	Deck(); //default constructor
	Deck(const Deck& d); //copy constructor
	~Deck();//destructor
	friend std::ostream& operator<<(std::ostream& out, const Deck& toOut); // stream operator overload
	Deck& operator =(const Deck& c); // assignment operator overload
	vector<Card*>cards;
	Card* draw();
	void printDeck();
	void shuffleDeck();
	void backToDeck(Card* c);
	void returnToDeck(Card* c);
};

class Hand {
public:
	Hand(); // default constructor
	Hand(const Hand& c); // copy constructor
	~Hand(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const Hand& toOut); // stream operator overload
	Hand& operator =(const Hand& c); // assignment operator overload
	void pickCard(Card* c);
	Card* removeFromHand(int index);
	vector<Card*> getCards();
	string printHand();
	Card* getCard();
	bool hasCards() { return this->cards.size() > 0; }

private:
	vector<Card*> cards;
};


class BombCard : public Card {
public:
	BombCard();// default constructor
	BombCard(string t); // parameterized constructor
	BombCard(const BombCard& c); // copy constructor
	~BombCard(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const BombCard& toOut); // stream operator overload
	BombCard& operator =(const BombCard& c); // assignment operator overload

	Order* getOrder();
};

class Reinforcement : public Card {
public:
	Reinforcement();// default constructor
	Reinforcement(string t); // parameterized constructor
	Reinforcement(const Reinforcement& c); // copy constructor
	~Reinforcement(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const Reinforcement& toOut); // stream operator overload
	Reinforcement& operator =(const Reinforcement& c); // assignment operator overload

	Order* getOrder();
};

class BlockadeCard : public Card {
public:
	BlockadeCard();// default constructor
	BlockadeCard(string t); // parameterized constructor
	BlockadeCard(const BlockadeCard& c); // copy constructor
	~BlockadeCard(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const BlockadeCard& toOut); // stream operator overload
	BlockadeCard& operator =(const BlockadeCard& c); // assignment operator overload

	Order* getOrder();
};


class AirliftCard : public Card {
public:
	AirliftCard();// default constructor
	AirliftCard(string t); // parameterized constructor
	AirliftCard(const AirliftCard& c); // copy constructor
	~AirliftCard(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const AirliftCard& toOut); // stream operator overload
	AirliftCard& operator =(const AirliftCard& c); // assignment operator overload

	Order* getOrder();
};

class Diplomacy : public Card {
public:
	Diplomacy();// default constructor
	Diplomacy(string t); // parameterized constructor
	Diplomacy(const Diplomacy& c); // copy constructor
	~Diplomacy(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const Diplomacy& toOut); // stream operator overload
	Diplomacy& operator =(const Diplomacy& c); // assignment operator overload


	Order* getOrder();
};
