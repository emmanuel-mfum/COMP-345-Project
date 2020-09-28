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
	string* eu = new string("Europe");
	string* fr = new string("France");

	Map* m = new Map(*mn);

	// add continents, and countries
	// continent needs to be added before countries on that continent are added
	m->addContinent(*na);
	m->addCountry(*na, *ca);
	m->addCountry(*na, *us);
	// add edge between countries, requires countries to be registered on the map
	m->addEdge(*ca, *us);

	m->addContinent(*eu);
	// once multiple continents on the map, the continents need to be connected subgraphs, so connect them
	m->addEdge(*eu, *na);
	
	m->addCountry(*eu, *fr);

	// validate
	bool isValidMap = m->validate();

	std::cout << "END!" << endl;
}