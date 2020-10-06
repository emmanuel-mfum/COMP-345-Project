#include "Cards.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>


using namespace std;

const int number_of_cards = 50;

Card::Card() {

}
Card::Card(string t) {
	// Maybe you should check to make sure the string is one of the proper types?
	// although if only the Deck is going to create a card that's probably not an issue
	 type = t; // set the type at creation
}

	
void Card:: play() {

	cout << "This is the effect of the card " << Card::getType() << endl;
}

string Card::getType() {
	return type; // returns the type
}

string Card::print() const {
	return ("The type of the card is " + type); // returns the type of the card
}



Deck::Deck() {
	string types[] = { "BombCard","Reinforcement","Blockade","Airlift","Diplomacy" }; // the 5 possible types of cards
	

	for (int i = 0; i < number_of_cards; i++) { // populates the cards vector with all for types
		cards.push_back(Card(types[i % 5]));
	}
	
}

void Deck::shuffleDeck() { // randomizes the deck


	for (int i = 0; i < number_of_cards; i++) {

		int random = rand() % number_of_cards;// generates a random number between 0 and 52

		Card temp = cards[i]; // store the object in temp variable
		cards[i] = cards.at(random); //picks an object at a random position in the deck and stores it inside the current position (index)
		cards[random] = temp; // stores the object in the temp into the position selected at random.
	}

	cout << "The deck has been shuffled for the players" << endl;
}

Card Deck::draw()
{
	cout << "The player will draw a card" << endl;
	
	// the assignment says random card, would it be possible to randomize this somehow?

	Card a = cards.back();// stores the card from the back of the deck
	cards.pop_back(); // remove the last card (the one just taken)
	cout << "The size of the deck is now " << cards.size() << endl; // indicates the remaining numbers of cards in the deck

	return a; // returns the card removed.
	
}

void Deck::printDeck()
{
	cout << left;

	for (int i = 0; i < cards.size(); i++) {
		cout <<cards.at(i).print() <<endl;
	}
}

void Deck::backToDeck(Card c) {

	cards.insert(cards.begin(), c);
	cout << "Element inserted into the deck " << endl;
}

Hand::Hand() {

};
Hand::Hand(Card c) {
	cards.push_back(c);
}

void Hand::pickCard(Card c) {
	cards.push_back(c);
}


 Card Hand::removeFromHand(int index) {

	int i = index;
	Card b = cards.at(i);
	cards.erase(cards.begin() + i);
	cout << "Card removed from hand !" << endl;
	return b;
}




