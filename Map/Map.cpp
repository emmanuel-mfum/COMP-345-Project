#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "Map.h"
#include "../Observer/Observer.h"


int MapComponent::componentCounter = 0;


using namespace std;


/* ========================================================================================================= */
/*
*
* MAPEDGE
* 
*/
MapEdge::MapEdge(MapComponent* territoryOne, MapComponent* territoryTwo) {
	this->territoryOne = territoryOne;
	this->territoryTwo = territoryTwo;
}

MapEdge::MapEdge(const MapEdge& source) {
	this->territoryOne = source.territoryOne;
	this->territoryTwo = source.territoryTwo;
}

MapEdge::~MapEdge() {
	// don't call delete, this object never creates the territories
	this->territoryOne = NULL;
	this->territoryTwo = NULL;
}

MapEdge& MapEdge::operator=(const MapEdge& rhs) {
	if (this == &rhs) {
		return *this;
	}

	// delete current references
	delete this->territoryOne;
	delete this->territoryTwo;

	// create new references
	this->territoryOne = rhs.territoryOne;
	this->territoryTwo = rhs.territoryTwo;
	return *this;
}

ostream& operator<<(ostream& out, const MapEdge& toOut) {
	out << "Edge:: FROM <> TO" << endl;
	out << toOut.territoryOne->getTerritoryName() << " <-----------> " << toOut.territoryTwo->getTerritoryName() << endl;
	return out;
}


MapComponent* MapEdge::getAdjacentTerritory(string territoryName) {
	if (this->territoryOne->getTerritoryName().compare(territoryName) == 0) {
		return this->territoryTwo;
	}
	else {
		return this->territoryOne;
	}
}

bool MapEdge::containsBothTerritories(MapComponent* terrOne, MapComponent* terrTwo) {
	// make sure it's not the same territory
	if (terrOne->getTerritoryName().compare(terrTwo->getTerritoryName()) == 0) {
		return false;
	}
	// check that either 
	//      territoryOne.name = terrOne.name AND territoryTwo.name = terrTwo.name
	//      OR
	//      territoryOne.name = terrTwo.name AND territoryTwo.name = terrOne.name
	if (
		(
			this->territoryOne->getTerritoryName().compare(terrOne->getTerritoryName()) == 0 &&
			this->territoryTwo->getTerritoryName().compare(terrTwo->getTerritoryName()) == 0
		) || 
		(
			this->territoryOne->getTerritoryName().compare(terrTwo->getTerritoryName()) == 0 &&
			this->territoryTwo->getTerritoryName().compare(terrOne->getTerritoryName()) == 0
		)
	) {
		return true;
	}
	return false;
}





/* ========================================================================================================= */
/*
*
*
* MAP COMPONENT
*
*
*/


int MapComponent::getAndUpdateIdForNew() {
	int newInt = MapComponent::componentCounter;
	MapComponent::componentCounter++;
	return newInt;
}

MapComponent::MapComponent(string territoryName, TerritoryType t) {
	this->territoryType = t;
	this->territoryId = MapComponent::getAndUpdateIdForNew();
	this->territoryName = territoryName;
	this->connections;
}

MapComponent::MapComponent(const MapComponent& source) {
	this->territoryType = source.territoryType;
	this->territoryId = MapComponent::getAndUpdateIdForNew();
	this->territoryName = source.territoryName;
	this->connections.clear();
	// copy the connections
	for (MapEdge* e : source.connections) {
		this->connections.push_back(e);
	}
}

MapComponent::~MapComponent() {
	// do not do anything, let Map delete all the territories and connections it created
}

MapComponent& MapComponent::operator=(const MapComponent& rhs) {
	if (this == &rhs) {
		return *this;
	}

	this->territoryType = rhs.territoryType;
	this->territoryId = rhs.territoryId;
	this->territoryName = rhs.territoryName;
	this->connections.clear();

	for (MapEdge* e : rhs.connections) {
		this->connections.push_back(e);
	}
	return *this;
}

ostream& operator<<(ostream& out, const MapComponent& toOut) {
	if ((const_cast<MapComponent&>(toOut)).getTerritoryType() == TerritoryType::Continent) {
		// out << (dynamic_cast<Continent*>(const_cast<MapComponent&>(toOut)));
		out << "This component is a Continent" << endl;
	}
	else {
		out << "This component is a Country" << endl;
	}
	return out;
}


bool MapComponent::territoryNameMatches(string territoryName) {
	return (this->territoryName.compare(territoryName) == 0);
}

void MapComponent::addEdge(MapEdge* edge) {
	this->connections.push_back(edge);
}

bool MapComponent::validate(int min) {
	return this->connections.size() >= min;
}

string MapComponent::getTerritoryName() {
	// give back a copy
	return string(this->territoryName);
}

vector<MapComponent*> MapComponent::getAdjacentTerritories() {
	vector<MapComponent*> adjacentTerritories;
	// loop through all the connections this MapComponent has
	for (int i = 0; i < this->connections.size(); i++) {
		MapEdge* edge = this->connections.at(i);
		// get the other MapComponent participating in the connection that is not this MapComponent
		MapComponent* adjacentTerr = edge->getAdjacentTerritory(this->territoryName);
		adjacentTerritories.push_back(adjacentTerr);
	}
	return adjacentTerritories;
}

vector<Country*> MapComponent::getAdjacentCountries() {
	vector<Country*> adjacentTerritories;
	if (this->territoryType != TerritoryType::Country) {
		return adjacentTerritories;
	}
	// loop through all the connections this Country has
	for (int i = 0; i < this->connections.size(); i++) {
		MapEdge* edge = this->connections.at(i);
		// get the other Country participating in the connection that is not this MapComponent
		MapComponent* adjacentTerr = edge->getAdjacentTerritory(this->territoryName);
		adjacentTerritories.push_back(dynamic_cast<Country*>(adjacentTerr));
	}
	return adjacentTerritories;
}

TerritoryType MapComponent::getTerritoryType() {
	return this->territoryType;
}

int MapComponent::getId() {
	return this->territoryId;
}




/* ========================================================================================================= */
/*
*
*
* CONTINENT
*
*
*/

Continent::Continent(string territoryName) : MapComponent(territoryName, TerritoryType::Continent) {
	this->vertices;
	this->continentBonus = 0;
}

Continent::Continent(const Continent& source) : MapComponent(source) {
	this->vertices;
	this->continentBonus = source.continentBonus;
	
	for (Country* c : source.vertices) {
		this->vertices.push_back(c);
	}
}

Continent::~Continent() {
	// Map will delete all the objects
}

Continent& Continent::operator=(const Continent& rhs) {
	if (this == &rhs) {
		return *this;
	}

	this->territoryType = rhs.territoryType;
	this->territoryId = rhs.territoryId;
	this->territoryName = rhs.territoryName;
	this->continentBonus = rhs.continentBonus;
	this->connections.clear();

	for (MapEdge* e : rhs.connections) {
		this->connections.push_back(e);
	}

	this->vertices.clear();

	for (Country* c : rhs.vertices) {
		this->vertices.push_back(c);
	}

	return *this;
}

ostream& operator<<(ostream& out, const Continent& toOut) {
	out << "\n============================\n";
	out << "Continent: " << toOut.territoryName << endl;
	out << "  Constituent countries: " << to_string(toOut.vertices.size()) << endl;
	out << "  Accessible continents: " << to_string(toOut.connections.size()) << endl;
	out << "============================\n";

	return out;
}


void Continent::addVertex(Country* country) {
	this->vertices.push_back(country);
}

int Continent::getNumCountries() {
	return this->vertices.size();
}



/* ========================================================================================================= */
/*
*
*
* COUNTRY
*
*
*/

Country::Country(string territoryName) : MapComponent(territoryName, TerritoryType::Country) {
	this->parent = nullptr;
	this->playerId = -1;
	this->armies = 0;
	this->owner = nullptr;
	this->armiesAdvancingDuringRound = 0;
}

Country::Country(const Country& source) : MapComponent(source) {
	this->parent = source.parent;
	this->playerId = source.playerId;
	this->armies = source.armies;
	this->owner = source.owner;
	this->armiesAdvancingDuringRound = source.armiesAdvancingDuringRound;
}

Country::~Country() {
	// map will delete the countries and continents
	this->parent = NULL;
}

Country& Country::operator=(const Country& rhs) {
	if (this == &rhs) {
		return *this;
	}

	this->territoryType = rhs.territoryType;
	this->territoryId = rhs.territoryId;
	this->territoryName = rhs.territoryName;
	this->connections.clear();
	this->owner = rhs.owner;

	for (MapEdge* e : rhs.connections) {
		this->connections.push_back(e);
	}

	this->parent = rhs.parent;
	this->playerId = rhs.playerId;
	this->armies = rhs.armies;
	this->armiesAdvancingDuringRound = rhs.armiesAdvancingDuringRound;
	return *this;
}

ostream& operator<<(ostream& out, const Country& toOut) {
	out << "\n============================\n";
	out << "Country: " << toOut.territoryName << endl;
	out << "  Member of: " << toOut.parent->getTerritoryName() << endl;
	out << "  Accessible countries: " << to_string(toOut.connections.size()) << endl;
	out << "============================\n";

	return out;
}

void Country::setParent(Continent* parent) {
	this->parent = parent;
}

void Country::setPlayerOwnership(Player* player) {
	if (player == nullptr) {
		this->playerId = -1;
		this->owner = nullptr;
	}
	else {
		if (this->owner != nullptr && this->playerId != player->getPlayerId()) {
			this->owner->lostCountry(this);
		}
		this->playerId = player->getPlayerId();
		this->owner = player;
	}
}

string Country::getContinentParentName() {
	return this->parent->getTerritoryName();
}

int Country::getPlayerOwnership() {
	return this->playerId;
}

int Country::getParentId() {
	return this->parent->getId();
}

int Country::getParentNumCountries() {
	return this->parent->getNumCountries();
}

void Country::setArmiesOnTerritory(int a) {
	this->armies = a;
	if (this->armies < 0) {
		cout << "UHOH";
	}
}

int Country::getArmiesOnTerritory() {
	return this->armies;
}

int Country::getParentBonus() {
	return this->parent->getBonus();
}

void Country::deployArmies(int numDeploying) {
	this->armies += numDeploying;
}

int Country::reduceArmies(int numLeavingToAttack) {
	if (numLeavingToAttack > this->armies) {
		numLeavingToAttack = this->armies;
	}
	this->armies -= numLeavingToAttack;
	if (this->armies < 0) {
		cout << "UHOH";
	}
	return numLeavingToAttack;
}

void Country::sustainOpponentLosses(int numLost) {
	if (this->playerId != -1) {
		this->owner->sustainedLosses(numLost);
	}
}

void Country::blockade() {
	this->owner->wasConquered(this->armies, this->territoryName);
	this->owner = nullptr;
	this->playerId = -1;
	this->armies *= 2;
}

void Country::initiateDiplomacy(int diplomacyId) {
	this->owner->initiateDiplomacy(diplomacyId);
}

bool Country::diplomaticallyBlocked(int playerId) {
	return this->owner->getNegotiator() == playerId;
}

void Country::bomb() {
	int survivingArmies = this->armies / 2;
	int bombedArmies = this->armies - survivingArmies;
	if (this->owner != nullptr) {
		this->owner->sustainedLosses(bombedArmies);
	}
	this->armies = survivingArmies;
}


/* ========================================================================================================= */
/*
*
*
* MAP
*
*
*/
Map::Map(string mapName) {
	this->mapName = mapName;
	this->vertices;
	this->edges;
	this->mapTerritories;
	this->registeredStatsObserver = nullptr;
}


Map::Map(string mapName, GameStatsObserver* observer) {
	this->mapName = mapName;
	this->vertices;
	this->edges;
	this->mapTerritories;
	this->registeredStatsObserver = observer;
	this->registeredStatsObserver->registerMap(this);
}


Map::Map(const Map& source) {
	this->mapName = source.mapName;

	// should the observer be copied?!
	this->registeredStatsObserver = source.registeredStatsObserver;

	this->vertices.clear();
	// copy continents
	for (Continent* c : source.vertices) {
		this->vertices.push_back(c);
	}

	this->edges.clear();
	// copy edges
	for (MapEdge* e : source.edges) {
		this->edges.push_back(e);
	}

	this->mapTerritories.clear();
	// copy all components
	for (MapComponent* t: source.mapTerritories) {
		this->mapTerritories.push_back(t);
	}
}

Map& Map::operator=(const Map& rhs) {
	if (this == &rhs) {
		return *this;
	}

	this->mapName = rhs.mapName;
	this->registeredStatsObserver = rhs.registeredStatsObserver;
	this->vertices.clear();
	// copy continents
	for (Continent* c : rhs.vertices) {
		this->vertices.push_back(c);
	}

	this->edges.clear();
	// copy edges
	for (MapEdge* e : rhs.edges) {
		this->edges.push_back(e);
	}

	this->mapTerritories.clear();
	// copy all components
	for (MapComponent* t : rhs.mapTerritories) {
		this->mapTerritories.push_back(t);
	}

	return *this;
}

ostream& operator<<(ostream& out, const Map& toOut) {
	out << "\n===================================\n";
	out << "MAP: " + toOut.mapName;
	out << "\n  Continents: " + to_string(toOut.vertices.size());
	out << "\n  Countries: " + to_string((toOut.mapTerritories.size() - toOut.vertices.size()));
	out << "\n===================================\n\n";
	return out;
}


Map::~Map() {
	// TODO REMEMBER WHEN USING NEW, THEN DELETE ALL
}



bool Map::territoryExists(string territoryName) {
	// check if the map contains a territory with this name already
	// Map relies on territoryName being unique!
	for (int i = 0; i < this->mapTerritories.size(); i++) {
		if (this->mapTerritories.at(i)->territoryNameMatches(territoryName)) {
			return true;
		}
	}

	return false;
}

void Map::addCountryByName(string continentName, string territoryName) {
	if (!this->territoryExists(continentName)) {
		// continent should be added first
		cout << "Can't add a country to a continent that doesn't exist!!" << endl;
		return;
	}

	// find the content
	MapComponent* continentAsComponent = this->mapTerritories.at(this->findTerritory(continentName));
	Continent* continent = dynamic_cast<Continent*>(continentAsComponent);
	// create and add new country
	Country* country = new Country(territoryName);
	this->addCountryByReference(continent, country);
}


int Map::findTerritory(string territoryName) {
	// find the index of the territory
	for (int i = 0; i < this->mapTerritories.size(); i++) {
		if (this->mapTerritories.at(i)->territoryNameMatches(territoryName)) {
			return i;
		}
	}
	return -1;
}

void Map::addCountryByReference(Continent* continent, Country* country) {
	if (!this->territoryExists(continent->getTerritoryName())) {
		// continent should be added first
		cout << "Can't add a country to a continent that doesn't exist!!" << endl;
		return;
	}

	if (this->territoryExists(country->getTerritoryName())) {
		cout << "Can't add a country with a duplicate name!!" << endl;
		return;
	}
	// register the country with the map
	this->mapTerritories.push_back(country);
	// let the country know what continent it's a part of
	country->setParent(continent);
	// add the country to the continent
	continent->addVertex(country);
	if (this->registeredStatsObserver != nullptr) {
		this->registeredStatsObserver->update();
	}
}

void Map::addContinentByName(string continentName) {
	if (this->territoryExists(continentName)) {
		cout << "Can't add a continent with a duplicate name!!" << endl;
		return;
	}
	Continent* continent = new Continent(continentName);

	this->addContinentByReference(continent);
}

void Map::addContinentByReference(Continent* continent) {
	if (!this->territoryExists(continent->getTerritoryName())) {
		// add country to the map
		this->mapTerritories.push_back(continent);
		// add the edge to the graph
		this->vertices.push_back(continent);
	}
}

void Map::addEdgeByName(string territoryNameOne, string territoryNameTwo) {
	if (this->territoryExists(territoryNameOne) && this->territoryExists(territoryNameTwo)) {
		// get the two territories that will make up the edge
		MapComponent* terrOne = this->mapTerritories.at(this->findTerritory(territoryNameOne));
		MapComponent* terrTwo = this->mapTerritories.at(this->findTerritory(territoryNameTwo));;

		if (terrOne->getTerritoryType() != terrTwo->getTerritoryType()) {
			cout << "The territories can't be connected because they are not of the same type!!" << endl;
		}
		else if (this->edgeExists(terrOne, terrTwo)) {
			// cout << "The territories were not connected because a connection already exists!!" << endl;
		}
		else {
			this->addEdgeByReference(terrOne, terrTwo);
		}
	}
	else {
		cout << "One of the territories does not exist on the map!!" << endl;
	}
}

void Map::addEdgeByReference(MapComponent* territoryOne, MapComponent* territoryTwo) {
	if (
		this->territoryExists(territoryOne->getTerritoryName()) &&
		this->territoryExists(territoryTwo->getTerritoryName()) &&
		territoryOne->getTerritoryType() == territoryTwo->getTerritoryType() &&
		!this->edgeExists(territoryOne, territoryTwo)
	) {
		// only allow edges between territories of the same type
		//    continent <-> continent
		//    country <-> country
		// connections between continents and countries are in the vertices of the continent
		// create the edge between the territories
		MapEdge* edge = new MapEdge(territoryOne, territoryTwo);
		// register the edge
		this->edges.push_back(edge);
		territoryOne->addEdge(edge);
		territoryTwo->addEdge(edge);
	}
}

bool Map::edgeExists(MapComponent* territoryOne, MapComponent* territoryTwo) {
	for (int i = 0; i < this->edges.size(); i++) {
		if (this->edges.at(i)->containsBothTerritories(territoryOne, territoryTwo)) {
			return true;
		}
	}
	return false;
}

bool Map::validate() {
	vector<Continent*> continents;
	vector<vector<Country*>*> countriesByContinent;

	// classify the territories
	for (int i = 0; i < this->mapTerritories.size(); i++) {
		// if continent, add to continents
		if (this->mapTerritories.at(i)->getTerritoryType() == TerritoryType::Continent) {
			continents.push_back(dynamic_cast<Continent*> (this->mapTerritories.at(i)));
			countriesByContinent.push_back(new vector<Country*>);
		}
		else {
			// add to countries
			int j;
			for (j = 0; j < continents.size(); j++) {
				if (continents.at(j)->getTerritoryName().compare((dynamic_cast<Country*>(this->mapTerritories.at(i)))->getContinentParentName()) == 0) {
					break;
				}
			}
			countriesByContinent.at(j)->push_back(dynamic_cast<Country*>(this->mapTerritories.at(i)));
		}
	}

	vector<int> currentIdxs(continents.size());


	// check each territory, make sure each territory has the minimum number of connections
	// (0 if there is only one territory of the type, 1 otherwise)
	// have to deal with case when country belongs to continent, but it is the only country belonging to the continent
	for (int i = 0, j = 0, k = 0; i < this->mapTerritories.size(); i++) {
		int min = 1;
		
		if (this->mapTerritories.at(i)->getTerritoryType() == TerritoryType::Continent) {
			min = (continents.size() > 1) ? min : 0;
			Continent* terr = continents.at(j);
			if (!terr->validate(min)) {
				return false;
			}
			j++;
		}
		else {
			int l;
			for (l = 0; l < continents.size(); l++) {
				if (continents.at(l)->getTerritoryName().compare((dynamic_cast<Country*>(this->mapTerritories.at(i)))->getContinentParentName()) == 0) {
					break;
				}
			}
			min = (countriesByContinent.at(l)->size() > 1) ? min : 0;
			Country* terr = countriesByContinent.at(l)->at(currentIdxs.at(l));
			if (!terr->validate(min)) {
				return false;
			}
			currentIdxs[l]++;
		}
	}
	return true;
}

vector<Country*> Map::getCountries() {
	// return all countries on the map
	vector<Country*> countries;
	for (MapComponent* m : this->mapTerritories) {
		if (m->getTerritoryType() == TerritoryType::Country) {
			countries.push_back(dynamic_cast<Country*>(m));
		}
	}
	return countries;
}

Country* Map::setPlayerOwnership(Player* player, string territoryName) {
	if (this->territoryExists(territoryName)) {
		Country* terr = dynamic_cast<Country*>(this->mapTerritories.at(this->findTerritory(territoryName)));
		terr->setPlayerOwnership(player);
		// return territory if found
		return terr;
	}
	return nullptr;
}

int Map::getNumContinents() {
	return this->vertices.size();
}

int Map::getNumCountries() {
	return this->mapTerritories.size() - this->vertices.size();
}

void Map::attachGameStatsObserver(GameStatsObserver* gso) {
	this->registeredStatsObserver = gso;
}

int Map::deservesContinentBonus(int playerId) {
	map<int, pair<int, int>> ownershipByContinent;
	map<int, int> contBonusMap;
	
	// determine how many countries on each continent the player owns
	for (MapComponent* c : this->mapTerritories) {
		if (c->getTerritoryType() == TerritoryType::Country) {
			Country* currentCountry = dynamic_cast<Country*>(c);
			if (currentCountry->getPlayerOwnership() == playerId) {
				if (ownershipByContinent.find(currentCountry->getParentId()) == ownershipByContinent.end()) {
					// not found
					pair<int, int> firstPair = pair<int, int>(currentCountry->getParentNumCountries(), 1);
					ownershipByContinent.insert(pair<int, pair<int, int>>(currentCountry->getParentId(), firstPair));
					contBonusMap.insert(pair<int, int>(currentCountry->getParentId(), currentCountry->getParentBonus()));
				}
				else {
					ownershipByContinent[currentCountry->getParentId()].second++;
				}
			}
		}
	}

	int totalBonus = 0;

	for (map<int, pair<int, int>>::iterator itr = ownershipByContinent.begin(); itr != ownershipByContinent.end(); ++itr) {
		// num territories owned on the continent equals num territories on the continent;
		if (itr->second.first == itr->second.second) {
			totalBonus += contBonusMap[itr->first];
		}
	}

	return totalBonus;
}

void Map::setContinentBonus(string continentName, int bonus) {
	for (int i = 0; i < this->mapTerritories.size(); i++) {
		if (
			this->mapTerritories[i]->getTerritoryType() == TerritoryType::Continent &&
			this->mapTerritories[i]->getTerritoryName().compare(continentName) == 0
		) {
			Continent* cont = dynamic_cast<Continent*>(this->mapTerritories[i]);
			cont->setBonus(bonus);
			break;
		}
	}
}
