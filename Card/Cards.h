#pragma once
#include <string>
#include <vector>
using namespace std;

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
private:
	string type;
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


};

class Reinforcement : public Card {
public:
	Reinforcement();// default constructor
	Reinforcement(string t); // parameterized constructor
	Reinforcement(const Reinforcement& c); // copy constructor
	~Reinforcement(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const Reinforcement& toOut); // stream operator overload
	Reinforcement& operator =(const Reinforcement& c); // assignment operator overload

};

class Blockade : public Card {
public:
	Blockade();// default constructor
	Blockade(string t); // parameterized constructor
	Blockade(const Blockade& c); // copy constructor
	~Blockade(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const Blockade& toOut); // stream operator overload
	Blockade& operator =(const Blockade& c); // assignment operator overload

};


class Airlift : public Card {
public:
	Airlift();// default constructor
	Airlift(string t); // parameterized constructor
	Airlift(const Airlift& c); // copy constructor
	~Airlift(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const Airlift& toOut); // stream operator overload
	Airlift& operator =(const Airlift& c); // assignment operator overload

};

class Diplomacy : public Card {
public:
	Diplomacy();// default constructor
	Diplomacy(string t); // parameterized constructor
	Diplomacy(const Diplomacy& c); // copy constructor
	~Diplomacy(); // destructor
	friend std::ostream& operator<<(std::ostream& out, const Diplomacy& toOut); // stream operator overload
	Diplomacy& operator =(const Diplomacy& c); // assignment operator overload

};
