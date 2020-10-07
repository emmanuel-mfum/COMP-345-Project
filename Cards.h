#pragma once
#include <string>
#include <vector>
using namespace std;

class Card {
public: 
	Card(Deck*deck,string t);
	Deck d;
	void play(); 
	string print() const;
	string getType();
private:
	string type;
};


class Deck {
public:
	Deck();
	vector<Card>cards;
	Card draw();
	void printDeck();
	void shuffleDeck();
	void backToDeck(Card c);

};

class Hand {
public:
	Hand();
	Hand(Card c);
	vector<Card>cards;
	void pickCard(Card c);
	Card removeFromHand(int index);
};

