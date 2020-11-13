#include <string>
#include <vector>
#include <iostream>
#include "Map.h"


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

MapEdge::~MapEdge() {
	delete this->territoryOne;
	delete this->territoryTwo;
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

MapComponent::MapComponent(string territoryName): MapComponent(territoryName, TerritoryType::Undefined) {

}

MapComponent::MapComponent(string territoryName, TerritoryType t) {
	this->territoryType = t;
	this->territoryId = MapComponent::getAndUpdateIdForNew();
	this->territoryName = territoryName;
	this->connections;
}


MapComponent::~MapComponent() {
	// maybe this should delete all connections and components? 
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
}

void Continent::addVertex(Country* country) {
	this->vertices.push_back(country);
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
}

void Country::setParent(Continent* parent) {
	this->parent = parent;
}

void Country::setPlayerOwnership(int playerId) {
	this->playerId = playerId;
}

string Country::getContinentParentName() {
	return this->parent->getTerritoryName();
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
}


Map::~Map() {

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
			cout << "The territories were not connected because a connection already exists!!" << endl;
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

Country* Map::setPlayerOwnership(int playerId, string territoryName) {
	if (this->territoryExists(territoryName)) {
		Country* terr = dynamic_cast<Country*>(this->mapTerritories.at(this->findTerritory(territoryName)));
		terr->setPlayerOwnership(playerId);
		// return territory if found
		return terr;
	}
	return nullptr;
}

/* 
string getDisplayString();
*/