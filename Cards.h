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

