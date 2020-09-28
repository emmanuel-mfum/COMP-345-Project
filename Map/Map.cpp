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
	this->mapTerritories = new vector<Territory>;
	this->vertices = new vector<Territory>;
	this->edges = new vector<MapEdge>;
}

Map::~Map() {
	delete this->edges;
	delete this->vertices;
	delete this->mapTerritories;
	delete this->mapName;
};

bool Map::territoryExists(string territoryName) {
	for (Territory& t : *this->mapTerritories) {
		if (t.territoryNameMatches(territoryName)) {
			return true;
		}
	}

	return false;
}

void Map::addTerritory(const TerritoryType &territoryType, string territoryName) {
	if (!this->territoryExists(territoryName)) {
		Territory *ter = new Territory(territoryType, territoryName);
		this->mapTerritories->push_back(*ter);
	}
}

void Map::addCountry(string continentName, string territoryName) {
	Territory* country = nullptr;
	

	if (!this->territoryExists(continentName)) {
		// continent should be added first
		return;
	}

	// find the content
	Territory* continent = &this->mapTerritories->at(this->findTerritory(continentName));
	// create a new country
	country = new Territory(TerritoryType::Country, territoryName);
	// register the country with the map
	this->mapTerritories->push_back(*country);
	// add the country to the continent
	continent->addVertex(*country);
}

void Map::addContinent(string continentName) {
	if (!this->territoryExists(continentName)) {
		Territory* continent = new Territory(TerritoryType::Continent, continentName);
		// add country to the map
		this->mapTerritories->push_back(*continent);
		// add the edge to the graph
		this->addVertex(*continent);
	}
}


void Map::addEdge(string territoryNameOne, string territoryNameTwo) {
	if (this->territoryExists(territoryNameOne) && this->territoryExists(territoryNameTwo)) {
		// get the two territories that will make up the edge
		Territory* terrOne = &this->mapTerritories->at(this->findTerritory(territoryNameOne));
		Territory* terrTwo = &this->mapTerritories->at(this->findTerritory(territoryNameTwo));;
		
		// create the edge 
		MapEdge* edge = new MapEdge(terrOne, terrTwo);

		// register the edge
		this->edges->push_back(*edge);
	}
}

void Map::addVertex(const Territory& territory) {
	this->vertices->push_back(territory);
}

int Map::findTerritory(string territoryName) {
	// find the territory
	int counter = 0;
	for (Territory& t : *this->mapTerritories) {
		if (t.territoryNameMatches(territoryName)) {
			return counter;
		}
		counter++;
	}
	return -1;
}






/*
* 
* TERRITORY
*/
Territory::Territory() {
	this->territoryType = nullptr;
	this->territoryName = nullptr;
	this->connections = new vector<MapEdge>;
	this->vertices = new vector<Territory>;
}


Territory::~Territory() {

}

Territory::Territory(const TerritoryType &territoryType, string territoryName) {
	this->connections = new vector<MapEdge>;
	this->territoryType = &(TerritoryType(territoryType));
	this->territoryName = new string(territoryName);

	if ((*this->territoryType) == TerritoryType::Continent) {
		this->vertices = new vector<Territory>;
	}
	else {
		this->vertices = nullptr;
	}
}


bool Territory::territoryNameMatches(string territoryName) {
	return ((*this->territoryName).compare(territoryName) == 0);
}

void Territory::addVertex(const Territory& territory) {
	this->vertices->push_back(territory);
}

void Territory::addEdge(const MapEdge& mapEdge) {
	this->connections->push_back(mapEdge);
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
