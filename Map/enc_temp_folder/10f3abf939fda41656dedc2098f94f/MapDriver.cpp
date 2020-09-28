#include <string>
#include <iostream>
#include "Map.h"

using namespace std;


int main() {
	std::cout << "HELLO" << endl;

	string* mn = new string("Risk Map");
	string* na = new string("North America");
	string* ca = new string("Canada");
	string* us = new string("United States");

	Map* m = new Map(*mn);

	m->addContinent(*na);
	m->addCountry(*na, *ca);
	m->addCountry(*na, *us);

	m->addEdge(*ca, *us);

	bool isValidMap = m->validate();

	std::cout << "END!" << endl;
}