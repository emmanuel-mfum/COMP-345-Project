#include <string>
#include <vector>
#include "Map.h"


using namespace std;


/*
*
* MAP
*/

Map::Map(string mapName) {
	this->mapName = new string(mapName);
	this->mapTerritories = new vector<Territory*>;
	this->vertices = new vector<Territory*>;
	this->edges = new vector<MapEdge*>;
}

Map::~Map() {
	delete this->edges;
	delete this->vertices;
	delete this->mapTerritories;
	delete this->mapName;
};

bool Map::territoryExists(string territoryName) {
	for (int i = 0; i < this->mapTerritories->size(); i++) {
		if (this->mapTerritories->at(i)->territoryNameMatches(territoryName)) {
			return true;
		}
	}

	return false;
}

void Map::addTerritory(const TerritoryType &territoryType, string territoryName) {
	if (!this->territoryExists(territoryName)) {
		Territory *ter = new Territory(territoryType, territoryName);
		this->mapTerritories->push_back(ter);
	}
}

void Map::addCountry(string continentName, string territoryName) {
	Territory* country = nullptr;
	

	if (!this->territoryExists(continentName)) {
		// continent should be added first
		return;
	}

	// find the content
	Territory* continent = this->mapTerritories->at(this->findTerritory(continentName));
	// create a new country
	country = new Territory(TerritoryType::Country, territoryName);
	// register the country with the map
	this->mapTerritories->push_back(country);
	// add the country to the continent
	continent->addVertex(country);
}

void Map::addContinent(string continentName) {
	if (!this->territoryExists(continentName)) {
		Territory* continent = new Territory(TerritoryType::Continent, continentName);
		// add country to the map
		this->mapTerritories->push_back(continent);
		// add the edge to the graph
		this->addVertex(continent);
	}
}


void Map::addEdge(string territoryNameOne, string territoryNameTwo) {
	if (this->territoryExists(territoryNameOne) && this->territoryExists(territoryNameTwo)) {
		// get the two territories that will make up the edge
		Territory* terrOne = this->mapTerritories->at(this->findTerritory(territoryNameOne));
		Territory* terrTwo = this->mapTerritories->at(this->findTerritory(territoryNameTwo));;
		
		// create the edge 
		MapEdge* edge = new MapEdge(terrOne, terrTwo);

		// register the edge
		this->edges->push_back(edge);
		terrOne->addEdge(edge);
		terrTwo->addEdge(edge);
	}
}

void Map::addVertex(Territory* territory) {
	this->vertices->push_back(territory);
}

int Map::findTerritory(string territoryName) {
	// find the territory
	for (int i = 0; i < this->mapTerritories->size(); i++) {
		if (this->mapTerritories->at(i)->territoryNameMatches(territoryName)) {
			return i;
		}
	}
	return -1;
}

bool Map::validate() {
	vector<Territory*>* continents = new vector<Territory*>;
	vector<Territory*>* countries = new vector<Territory*>;

	// classify the territories
	for (int i = 0; i < this->mapTerritories->size(); i++) {
		if (this->mapTerritories->at(i)->isContinent()) {
			continents->push_back(this->mapTerritories->at(i));
		}
		else {
			countries->push_back(this->mapTerritories->at(i));
		}
	}

	for (int i = 0, j = 0, k = 0; i < this->mapTerritories->size(); i++) {
		int min = 1;
		vector<Territory*>* terrs = nullptr;
		if (this->mapTerritories->at(i)->isContinent()) {
			min = (continents->size() > 1) ? min : 0;
			Territory* terr = continents->at(j);
			if (!terr->validate(min)) {
				return false;
			}
			j++;
		}
		else {
			min = (countries->size() > 1) ? min : 0;
			Territory* terr = countries->at(k);
			if (!terr->validate(min)) {
				return false;
			}
			k++;
		}
	}
	return true;
}






/*
* 
* TERRITORY
*/
Territory::Territory() {
	this->territoryType = nullptr;
	this->territoryName = nullptr;
	this->connections = new vector<MapEdge*>;
	this->vertices = new vector<Territory*>;
}


Territory::~Territory() {

}

Territory::Territory(const TerritoryType &territoryType, string territoryName) {
	this->connections = new vector<MapEdge*>;
	this->territoryType = &(TerritoryType(territoryType));
	this->territoryName = new string(territoryName);

	if ((*this->territoryType) == TerritoryType::Continent) {
		this->territoryType = new TerritoryType{ TerritoryType::Continent };
		this->vertices = new vector<Territory*>;
	}
	else {
		this->territoryType = new TerritoryType{ TerritoryType::Country };
		this->vertices = nullptr;
	}
}


bool Territory::territoryNameMatches(string territoryName) {
	return ((*this->territoryName).compare(territoryName) == 0);
}

void Territory::addVertex(Territory* territory) {
	this->vertices->push_back(territory);
}

void Territory::addEdge(MapEdge* mapEdge) {
	this->connections->push_back(mapEdge);
}

bool Territory::isContinent() {
	return (*this->territoryType) == TerritoryType::Continent;
}

bool Territory::validate(int min) {
	return this->connections->size() >= min;
}






/*
* 
* MAPEDGE
*/
MapEdge::MapEdge(Territory* territoryOne, Territory* territoryTwo) {
	this->territoryOne = territoryOne;
	this->territoryTwo = territoryTwo;
}

MapEdge::~MapEdge() {

}
