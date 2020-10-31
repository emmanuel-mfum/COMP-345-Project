#include <string>
#include <vector>
#include <iostream>

#include "Map.h"


using namespace std;


int main() {

	Map* m = new Map("Risk Map");

	Country* france = new Country("France");
	Continent* europe = new Continent("Europe");
	Country* uk = new Country("United Kingdom");
	Country* canada = new Country("Canada");
	Continent* northAmerica = new Continent("North America");
	Country* kenya = new Country("Kenya");
	Country* ethiopia = new Country("Ethiopia");
	Continent* africa = new Continent("Africa");
	Country* germany = new Country("Germany");


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

	Country* e = m->setPlayerOwnership(25, "Ethiopia");

	Country* g = m->setPlayerOwnership(41, "Germany");

	// true!!!!
	isValidMap = m->validate();

	cout << "END" << endl;



	return 0;
}