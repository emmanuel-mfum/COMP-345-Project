#pragma once
#include <string>
#include <vector>
using namespace std;


/*
* 
* Each class is missing: Destructor, operator=, and stream insertion operator
* 
*/

class Card {
public: 
	Card(string t);
	void play(); 
	string print() const;
	Card();
	string getType();
private:
	string type;
};


class Deck {
public:
	Deck();
	// in one of the recent lectures Nora said all member variables should be private 
	// as part of good OOP style
	vector<Card>cards;
	Card draw();
	void printDeck();
	void shuffleDeck();
	void backToDeck(Card c);

};



/*
* 
* I'm wondering if you need to create a Hand class? Should that be something that has more to do with the Player class?
*/
class Hand {
public:
	Hand();
	Hand(Card c);
	// in one of the recent lectures Nora said all member variables should be private 
	// as part of good OOP style
	vector<Card>cards;
	void pickCard(Card c);
	Card removeFromHand(int index);
};

