#include <string>
#include <vector>
#include <iostream>


#include "Observer.h"
#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"


using namespace std;


int main() {


	Map* m = new Map("Risk Map");

	GameStatsObserver* gso = new GameStatsObserver();

	m->attachGameStatsObserver(gso);
	gso->registerMap(m);

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

	// true!!!!
	isValidMap = m->validate();

	GameEngine engine;

	vector<Player*> players;
	players.push_back(new Player());
	players.push_back(new Player());

	engine.smallGameStart(m, players);

	Country* e = m->setPlayerOwnership(0, "Ethiopia");

	Country* g = m->setPlayerOwnership(1, "Germany");

	africa->setBonus(5);

	e = m->setPlayerOwnership(0, "Kenya");

	engine.reinforcementPhase();
	engine.issueOrdersPhase();
	engine.executeOrdersPhase();


	return 0;
}