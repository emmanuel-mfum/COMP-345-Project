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

	// add continents, and countries
	// continent needs to be added before countries on that continent are added
	m->addContinent(*na);
	m->addCountry(*na, *ca);
	m->addCountry(*na, *us);
	// add edge between countries, requires countries to be registered on the map
	m->addEdge(*ca, *us);

	// validate
	bool isValidMap = m->validate();

	std::cout << "END!" << endl;
}