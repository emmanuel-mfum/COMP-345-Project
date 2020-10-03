#include <iostream>
#include "Player.h"
#include "../Map/Map.h"

using namespace std;

int main() {
	// (copied this from MapDriver.cpp)
	// create a map object
	Map* m = new Map("Risk Map");
	// create some countries
	Territory* france = new Territory(TerritoryType::Country, "France");
	Territory* europe = new Territory(TerritoryType::Continent, "Europe");
	Territory* uk = new Territory(TerritoryType::Country, "United Kingdom");
	Territory* canada = new Territory(TerritoryType::Country, "Canada");
	Territory* northAmerica = new Territory(TerritoryType::Continent, "North America");
	Territory* kenya = new Territory(TerritoryType::Country, "Kenya");
	Territory* ethiopia = new Territory(TerritoryType::Country, "Ethiopia");
	Territory* africa = new Territory(TerritoryType::Continent, "Africa");
	Territory* germany = new Territory(TerritoryType::Country, "Germany");

	// add continents, and countries
	// continent needs to be added before countries on that continent are added
	m->addContinentByReference(northAmerica);
	m->addContinentByReference(europe);
	m->addEdgeByReference(northAmerica, europe);
	// add countries
	m->addCountryByReference(northAmerica, canada);
	m->addCountryByReference(europe, france);
	m->addCountryByReference(europe, uk);
	m->addEdgeByReference(france, uk);
	m->addContinentByReference(africa);
	m->addCountryByReference(europe, germany);
	m->addCountryByReference(africa, kenya);
	m->addCountryByReference(africa, ethiopia);
	m->addEdgeByReference(ethiopia, kenya);
	m->addEdgeByReference(africa, europe);
	m->addEdgeByReference(germany, france);

	// M is now fully valid map

	// set the worldMap of the player class to the new map
	Player::setMap(m);
	// create some player objects
	// all player objects can now call member functions 
	Player* aPlayer = new Player();
	aPlayer->declareOwner("Canada");
}