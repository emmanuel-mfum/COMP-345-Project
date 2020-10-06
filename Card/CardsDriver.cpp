#include <iostream>
#include<string>
#include "Cards.h"

using namespace std;


int main() {


	Deck deck;

	

	deck.printDeck(); // prints the whole deck
	deck.shuffleDeck(); // shuffles the deck
	cout << endl << endl;
	deck.printDeck(); // prints the deck after shuffling
	cout << endl << endl;
	Card a = deck.draw(); // draw a card from the deck
	cout << a.print() << "The last element" << endl; // prints the last element of the list (that was just drawn)

	Hand hand; // creates a Hand object

	hand.pickCard(a); // place the first drawn card in the hand
	cout << hand.cards.at(0).print(); // prints that card

	Card b = deck.draw(); // draws a second object from the deck
	hand.pickCard(b); // place the second drawn card into the deck

	Card c = deck.draw(); // draws a third object from the deck
	hand.pickCard(c); // place the third drawn card into the deck

	Card d = deck.draw(); // draws a fourth object from the deck
	hand.pickCard(d); // place the fourth drawn card into the deck

	Card e = deck.draw(); // draws a fifth object from the deck
	hand.pickCard(e); // place the fifth drawn card into the deck
	
	for (int i = 0; i < hand.cards.size(); i++) {
		cout << hand.cards.at(i).print() << endl; // prints all the cards in the hand
	}

	for (int i = 0; i < hand.cards.size(); i++) {
		hand.cards.at(i).play(); // prints all the cards in the hand
	}
	Card f = hand.removeFromHand(0);


	for (int i = 0; i < hand.cards.size(); i++) {
		cout << hand.cards.at(i).print() << endl; // prints all the cards in the hand
	}

	deck.backToDeck(f);

	deck.printDeck();
	return 0;


}