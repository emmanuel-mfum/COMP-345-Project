#define _DEBUG
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

#include <iostream>
#include "Cards.h"

using namespace std;

int main() {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif



	Deck* deck = new Deck(); // creates a new deck

	

	deck->printDeck(); // prints the whole deck
	cout << endl << endl;
	deck->shuffleDeck(); // shuffles the deck
	cout << endl << endl;
	deck->printDeck(); // prints the deck after shuffling
	cout << endl << endl;
	Card* a = deck->draw(); // draw a card from the deck
	cout << a->print() << " It was the last  last element of the deck" << endl; // prints the last element of the list (that was just drawn)

	Hand* hand = new Hand(); // creates a Hand object

	hand->pickCard(a); // place the first drawn card in the hand

	
	Card* cardx = hand->getCards().at(0);
	 

	if (cardx) // null pointer check
	{
		
		cout << cardx->print() << endl; // prints that card
	}
	else
	{
		cout << "Null pointer is occuring !" << endl;
	}

	

	

	Card* b = deck->draw(); // draws a second object from the deck
	hand->pickCard(b); // place the second drawn card into the hand



	Card* c = deck->draw(); // draws a third object from the deck
	hand->pickCard(c); // place the third drawn card into the hand



	Card* d = deck->draw(); // draws a fourth object from the deck
	hand->pickCard(d); // place the fourth drawn card into the hand



	Card* e = deck->draw(); // draws a fifth object from the deck
	hand->pickCard(e); // place the fifth drawn card into the hand



	cout << endl;

	cout << "The hand will now be printed " << endl;
	cout << endl << endl;
	
	for (int i = 0; i < hand->getCards().size(); i++) {// prints all the cards in the hand
		
		 Card* cardy = hand->getCards().at(i);

		 if (cardy) {
			 cout << cardy->print() << endl;
		 }
		 else {
			 cout << "This is a null pointer";
		 }
	
	}

	

	cout << endl;

	for (int i = 0; i < hand->getCards().size(); i++) {
		 // plays all the cards in the hand
		Card* cardp = hand->getCards().at(i);

		if (cardp) {
			cardp->play();
		}
		else {
			cout << "This is a null pointer";
		}
		
	}
	

	Card* f = hand->removeFromHand(0); // gets the first card in the hand
	Card* g = hand->removeFromHand(0);// gets the second card in the hand
	Card* h = hand->removeFromHand(0); // gets the third card in the hand
	Card* k = hand->removeFromHand(0); // gets the fourth card in the hand
	Card* m = hand->removeFromHand(0); // gets the fifth card in the hand
	cout << endl;

	cout << "The hand will be printed again ." << endl;

	for (int i = 0; i < hand->getCards().size(); i++) {// prints all the cards in the hand
		Card* cardInHand = hand->getCards().at(i);

		if (cardInHand) {
			cout << cardInHand->print() << endl;
		}
		else {
			cout << "Null pointer or invalid pointer occuring" << endl;
		}
	}

	cout << endl;
	deck->backToDeck(f); // puts back the card into the deck
	deck->backToDeck(g); // puts back the card into the deck
	deck->backToDeck(h); // puts back the card into the deck
	deck->backToDeck(k); // puts back the card into the deck
	deck->backToDeck(m); // puts back the card into the deck

	
	cout << endl;
	cout << "The deck will now be printed" << endl;
	cout << endl;

	deck->printDeck(); // prints the whole deck
	cout << endl << endl;
	cout << "Deleting pointers" << endl;

	// delete pointers , deck and delete hand

	
	delete deck;
	deck = nullptr;
	delete hand;
	hand = nullptr;
	
	return 0;


}
