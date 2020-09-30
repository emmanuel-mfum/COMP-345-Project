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
	m->addContinentByName(*na);
	m->addCountryByName(*na, *ca);
	m->addCountryByName(*na, *us);
	// add edge between countries, requires countries to be registered on the map
	m->addEdgeByName(*ca, *us);

	m->addContinentByName(*eu);
	// once multiple continents on the map, the continents need to be connected subgraphs, so connect them
	m->addEdgeByName(*eu, *na);
	
	m->addCountryByName(*eu, *fr);

	m->setPlayerOwnership(25, *fr);

	Territory* france = new Territory(TerritoryType::Country, *fr);
	Territory* europe = new Territory(TerritoryType::Continent, *eu);
	Territory* uk = new Territory(TerritoryType::Country, "United Kingdom");
	MapEdge* chunnel = new MapEdge(france, uk);
	europe->addVertex(france);
	france->setParent(europe);
	france->addEdge(chunnel);
	uk->addEdge(chunnel);

	chunnel = chunnel;

	MapEdge* reverseChunnel = new MapEdge(uk, france);
	reverseChunnel = chunnel;

	string franceName = france->getTerritoryName();

	Territory* franceCopy = new Territory(*france);
	france->setPlayerOwnership(25);
	Territory* franceCopy2 = new Territory(*france);

	TerritoryType tt = franceCopy->getTerritoryType();

	Map* m2 = new Map(*m);

	franceCopy2 = uk;


	// validate
	bool isValidMap = m->validate();

	std::cout << "END!" << endl;
}