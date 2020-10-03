#include <string>
#include <vector>
#include <iostream>
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

Map::Map(const Map& source) {
	this->mapName = new string(*source.mapName);
	this->mapTerritories = new vector<Territory*>;
	this->vertices = new vector<Territory*>;
	this->edges = new vector<MapEdge*>;

	// copy elements over
	for (int i = 0; i < source.mapTerritories->size(); i++) {
		this->mapTerritories->push_back(source.mapTerritories->at(i));
	}

	for (int i = 0; i < source.vertices->size(); i++) {
		this->vertices->push_back(source.vertices->at(i));
	}

	for (int i = 0; i < source.edges->size(); i++) {
		this->edges->push_back(source.edges->at(i));
	}
}

Map::~Map() {
	// Map should delete all the objects as well ??
	delete this->edges;
	delete this->vertices;
	delete this->mapTerritories;
	delete this->mapName;


};


Map& Map::operator=(const Map& rhs) {
	if (this == &rhs) {
		return *this;
	}

	delete this->edges;
	delete this->vertices;
	delete this->mapTerritories;
	delete this->mapName;

	this->mapName = new string(*rhs.mapName);
	this->mapTerritories = new vector<Territory*>;
	this->vertices = new vector<Territory*>;
	this->edges = new vector<MapEdge*>;

	// copy elements over
	for (int i = 0; i < rhs.mapTerritories->size(); i++) {
		this->mapTerritories->push_back(rhs.mapTerritories->at(i));
	}

	for (int i = 0; i < rhs.vertices->size(); i++) {
		this->vertices->push_back(rhs.vertices->at(i));
	}

	for (int i = 0; i < rhs.edges->size(); i++) {
		this->edges->push_back(rhs.edges->at(i));
	}

	return *this;
}

std::ostream& operator<<(std::ostream& out, const Map& toOut) {
	out << "\n===================================\n";
	out << "MAP: " + *toOut.mapName;
	out << "\n  Continents: " + to_string(toOut.vertices->size());
	out << "\n  Countries: " + to_string((toOut.mapTerritories->size() - toOut.vertices->size()));
	out << "\n===================================\n\n";
	return out;
}

bool Map::territoryExists(string territoryName) {
	// check if the map contains a territory with this name already
	// right now Map relies on territoryName being unique!
	for (int i = 0; i < this->mapTerritories->size(); i++) {
		if (this->mapTerritories->at(i)->territoryNameMatches(territoryName)) {
			return true;
		}
	}

	return false;
}

void Map::addTerritoryByName(const TerritoryType &territoryType, string territoryName) {
	// this method shouldn't be used....
	Territory *ter = new Territory(territoryType, territoryName);
	this->addTerritoryByReference(ter);
}

void Map::addTerritoryByReference(Territory* territory) {
	// this method also shouldn't be used. . . .
	if (!this->territoryExists(territory->getTerritoryName())) {
		this->mapTerritories->push_back(territory);
	}
}

void Map::addCountryByName(string continentName, string territoryName) {

	if (!this->territoryExists(continentName)) {
		// continent should be added first
		return;
	}

	// find the content
	Territory* continent = this->mapTerritories->at(this->findTerritory(continentName));
	// create a new country
	Territory* country = new Territory(TerritoryType::Country, territoryName);
	this->addCountryByReference(continent, country);
}

void Map::addCountryByReference(Territory* continent, Territory* country) {
	if (!this->territoryExists(continent->getTerritoryName())) {
		// continent should be added first
		return;
	}

	if (this->territoryExists(country->getTerritoryName())) {
		// country already exists on the map!
		return;
	}
	// register the country with the map
	this->mapTerritories->push_back(country);
	// let the country know what continent it's a part of
	country->setParent(continent);
	// add the country to the continent
	continent->addVertex(country);
}

void Map::addContinentByName(string continentName) {
	Territory* continent = new Territory(TerritoryType::Continent, continentName);

	this->addContinentByReference(continent);
}

void Map::addContinentByReference(Territory* continent) {
	if (!this->territoryExists(continent->getTerritoryName())) {
		// add country to the map
		this->mapTerritories->push_back(continent);
		// add the edge to the graph
		this->addVertex(continent);
	}
}


void Map::addEdgeByName(string territoryNameOne, string territoryNameTwo) {
	if (this->territoryExists(territoryNameOne) && this->territoryExists(territoryNameTwo)) {
		// get the two territories that will make up the edge
		Territory* terrOne = this->mapTerritories->at(this->findTerritory(territoryNameOne));
		Territory* terrTwo = this->mapTerritories->at(this->findTerritory(territoryNameTwo));;
		
		this->addEdgeByReference(terrOne, terrTwo);
	}
}

void Map::addEdgeByReference(Territory* territoryOne, Territory* territoryTwo) {
	if (
		this->territoryExists(territoryOne->getTerritoryName()) && 
		this->territoryExists(territoryTwo->getTerritoryName()) &&
		territoryOne->getTerritoryType() == territoryTwo->getTerritoryType()
	) {
		// only allow edges between territories of the same type
		//    continent <-> continent
		//    country <-> country
		// connections between continents and countries are in the vertices of the continent
		// create the edge between the territories
		MapEdge* edge = new MapEdge(territoryOne, territoryTwo);

		// register the edge
		this->edges->push_back(edge);
		territoryOne->addEdge(edge);
		territoryTwo->addEdge(edge);
	}
}

void Map::addVertex(Territory* territory) {
	this->vertices->push_back(territory);
}

int Map::findTerritory(string territoryName) {
	// find the index of the territory
	for (int i = 0; i < this->mapTerritories->size(); i++) {
		if (this->mapTerritories->at(i)->territoryNameMatches(territoryName)) {
			return i;
		}
	}
	return -1;
}

bool Map::validate() {
	vector<Territory*>* continents = new vector<Territory*>;
	vector<vector<Territory*>*>* countriesByContinent = new vector<vector<Territory*>*>;

	// classify the territories
	for (int i = 0; i < this->mapTerritories->size(); i++) {
		if (this->mapTerritories->at(i)->isContinent()) {
			continents->push_back(this->mapTerritories->at(i));
			countriesByContinent->push_back(new vector<Territory*>);
		}
		else {
			int j;
			for (j = 0; j < continents->size(); j++) {
				if (continents->at(j)->getTerritoryName().compare(this->mapTerritories->at(i)->getContinentName()) == 0) {
					break;
				}
			}
			countriesByContinent->at(j)->push_back(this->mapTerritories->at(i));
		}
	}

	vector<int> currentIdxs (continents->size());


	// check each territory, make sure each territory has the minimum number of connections
	// (0 if there is only one territory of the type, 1 otherwise)
	// have to deal with case when country belongs to continent, but it is the only country belonging to the continent
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
			int l;
			for (l = 0; l < continents->size(); l++) {
				if (continents->at(l)->getTerritoryName().compare(this->mapTerritories->at(i)->getContinentName()) == 0) {
					break;
				}
			}
			min = (countriesByContinent->at(l)->size() > 1) ? min : 0;
			Territory* terr = countriesByContinent->at(l)->at(currentIdxs.at(l));
			if (!terr->validate(min)) {
				return false;
			}
			currentIdxs[l]++;
		}
	}
	return true;
}

Territory* Map::setPlayerOwnership(int playerId, string territoryName) {
	if (this->territoryExists(territoryName)) {
		Territory* terr = this->mapTerritories->at(this->findTerritory(territoryName));
		terr->setPlayerOwnership(playerId);
		// return territory if found
		return terr;
	}
	return nullptr;
}

string const& Map::getDisplayStringForOut() const {
	string displayString = "\n===================================\n";
	displayString.append("MAP: " + *this->mapName);
	displayString.append("\n  Continents: " + to_string(this->vertices->size()));
	displayString.append("\n  Countries: " + to_string((this->mapTerritories->size() - this->vertices->size())));
	displayString.append("\n===================================\n\n");
	return displayString;
}

string Map::getDisplayString() {
	string displayString = "\n===================================\n";
	displayString.append("MAP: " + *this->mapName);
	displayString.append("\n  Continents: " + to_string(this->vertices->size()));
	displayString.append("\n  Countries: " + to_string((this->mapTerritories->size() - this->vertices->size())));
	displayString.append("\n===================================\n\n");
	return displayString;
}






/*
* 
* TERRITORY
*/

Territory::Territory(const TerritoryType &territoryType, string territoryName) {
	this->connections = new vector<MapEdge*>;
	this->territoryType = &(TerritoryType(territoryType));
	this->territoryName = new string(territoryName);
	this->parent = nullptr;
	this->ownerId = nullptr;

	if ((*this->territoryType) == TerritoryType::Continent) {
		this->territoryType = new TerritoryType{ TerritoryType::Continent };
		this->vertices = new vector<Territory*>;
	}
	else {
		this->territoryType = new TerritoryType{ TerritoryType::Country };
		this->vertices = nullptr;
	}
}

Territory::Territory(const Territory& source) {
	// the copy constructor
	this->territoryType = &(TerritoryType(*source.territoryType));
	this->territoryName = new string(*source.territoryName);
	this->parent = source.parent;
	if (source.ownerId == nullptr) {
		this->ownerId = nullptr;
	}
	else {
		this->ownerId = new int(*source.ownerId);
	}
	this->connections = new vector<MapEdge*>;
	for (int i = 0; i < source.connections->size(); i++) {
		// copy connections
		this->connections->push_back(source.connections->at(i));
	}
	

	if ((*this->territoryType) == TerritoryType::Continent) {
		this->territoryType = new TerritoryType{ TerritoryType::Continent };
		this->vertices = new vector<Territory*>;
		// copy the references to vertices
		for (int i = 0; i < source.vertices->size(); i++) {
			this->vertices->push_back(source.vertices->at(i));
		}
	}
	else {
		this->territoryType = new TerritoryType{ TerritoryType::Country };
		this->vertices = nullptr;
	}
}

Territory::~Territory() {
	delete this->territoryName;
	delete this->territoryType;
	delete this->vertices;
	delete this->connections;
	delete this->parent;
	delete this->ownerId;
}


ostream& operator<<(ostream& out, const Territory& toOut) {
	if (*toOut.territoryType == TerritoryType::Continent) {
		cout << "\n============================\n";
		cout << "Continent: " << *toOut.territoryName << endl;
		cout << "  Constituent countries: " << to_string(toOut.vertices->size()) << endl;
		cout << "  Accessible continents: " << to_string(toOut.connections->size()) << endl;
		cout << "============================\n";
	}
	else {
		cout << "\n============================\n";
		cout << "Country: " << *toOut.territoryName << endl;
		cout << "  Member of: " << toOut.parent->getTerritoryName() << endl;
		cout << "  Accessible countries: " << to_string(toOut.connections->size()) << endl;
		cout << "============================\n";
	}
	return out;
}

Territory& Territory::operator=(const Territory& rhs) {
	if (this == &rhs) {
		return *this;
	}

	delete this->territoryType;
	delete this->territoryName;
	delete this->parent;
	delete this->ownerId;
	delete this->vertices;
	delete this->connections;
	// how to avoid copying this code across constructors. . . . ?!
	this->territoryType = &(TerritoryType(*rhs.territoryType));
	this->territoryName = new string(*rhs.territoryName);
	this->parent = rhs.parent;
	if (rhs.ownerId == nullptr) {
		this->ownerId = nullptr;
	}
	else {
		this->ownerId = new int(*rhs.ownerId);
	}
	this->connections = new vector<MapEdge*>;
	for (int i = 0; i < rhs.connections->size(); i++) {
		// copy connections
		this->connections->push_back(rhs.connections->at(i));
	}


	if ((*this->territoryType) == TerritoryType::Continent) {
		this->territoryType = new TerritoryType{ TerritoryType::Continent };
		this->vertices = new vector<Territory*>;
		// copy the references to vertices
		for (int i = 0; i < rhs.vertices->size(); i++) {
			this->vertices->push_back(rhs.vertices->at(i));
		}
	}
	else {
		this->territoryType = new TerritoryType{ TerritoryType::Country };
		this->vertices = nullptr;
	}

	return *this;
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

void Territory::setParent(Territory* parent) {
	this->parent = parent;
}

void Territory::setPlayerOwnership(int playerId) {
	this->ownerId = new int(playerId);
}

string Territory::getTerritoryName() {
	// give back a copy
	return *new string(*this->territoryName);
}

TerritoryType Territory::getTerritoryType() {
	// give back a copy
	return *new TerritoryType(*this->territoryType);
}

string Territory::getContinentName() {
	return this->parent->getTerritoryName();
}

vector<Territory*>* Territory::getAdjacentTerritories() {
	vector<Territory*>* adjacentTerritories = new vector<Territory*>;
	// loop through all the connections this territory has
	for (int i = 0; i < this->connections->size(); i++) {
		MapEdge* edge = this->connections->at(i);
		// get the other territory participating in the connection that is not this territory
		Territory* adjacentTerr = edge->getAdjacentTerritory(*this->territoryName);
		adjacentTerritories->push_back(adjacentTerr);
	}
	return adjacentTerritories;
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
	delete this->territoryOne;
	delete this->territoryTwo;
}

MapEdge::MapEdge(const MapEdge& source) {
	this->territoryOne = new Territory(*source.territoryOne);
	this->territoryTwo = new Territory(*source.territoryTwo);
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


Territory* MapEdge::getAdjacentTerritory(string territoryName) {
	if (this->territoryOne->getTerritoryName().compare(territoryName) == 0) {
		return this->territoryTwo;
	}
	else {
		return this->territoryOne;
	}
}