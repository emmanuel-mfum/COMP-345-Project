#include <string>
#include <iostream>
#include "Map.h"

using namespace std;


int main() {
	std::cout << "HELLO" << endl;


	Map* m = new Map("Risk Map");

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
	// validate
	// currently false, the continents are not connected!!!
	bool isValidMap = m->validate();
	m->addEdgeByReference(northAmerica, europe);

	// validate
	// true because the continents are connected
	isValidMap = m->validate();
	// add countries
	m->addCountryByReference(northAmerica, canada);
	m->addCountryByReference(europe, france);
	m->addCountryByReference(europe, uk);

	// validate
	// false because the countries on the same continent are not connected!!
	isValidMap = m->validate();
	// connect countries on the same continent
	m->addEdgeByReference(france, uk);
	// validate
	// true because the countries on the same continent are now connected!!
	isValidMap = m->validate();

	m->addContinentByReference(africa);
	m->addCountryByReference(europe, germany);
	m->addCountryByReference(africa, kenya);
	m->addCountryByReference(africa, ethiopia);
	// validate
	// false because none of new objects connected!!
	isValidMap = m->validate();

	m->addEdgeByReference(ethiopia, kenya);

	// false because africa not part of continent graph, germany not part of europe graph!!
	isValidMap = m->validate();

	m->addEdgeByReference(africa, europe);

	// false because germany not part of europe graph!!
	isValidMap = m->validate();

	m->addEdgeByReference(germany, france);

	m->setPlayerOwnership(25, "ethiopia");

	m->setPlayerOwnership(41, "germany");

	// true!!!!
	isValidMap = m->validate();

	std::cout << *m;

	cout << *africa;
	cout << *northAmerica;
	cout << *europe;
	cout << *canada;
	cout << *kenya;

	std::cout << "END!" << endl;
}