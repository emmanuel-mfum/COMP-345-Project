#pragma once

#include <string>
#include <vector>


using namespace std;
/**
*
* This file contains Map, Territory, Country, Continent, and MapEdge classes
*
*/

// let the compiler know these classes are defined in this file
class MapEdge;
class Territory;
class Map;


enum class TerritoryType {
	Country = 1,
	Continent = 2,
};





/*
*
* MapEdge represents connection between two MapVertices
*/
class MapEdge {
public:
	MapEdge(Territory* territoryOne, Territory* territoryTwo);
	MapEdge(const MapEdge& source);
	
	~MapEdge();

	MapEdge& operator=(const MapEdge& rhs);

private:
	Territory* territoryOne;
	Territory* territoryTwo;
};


/*
*
* Territory is the class that represents a vertex in the graph
*/
class Territory {
public:
	Territory(const TerritoryType& territoryType, string territoryName);
	Territory(const Territory& source);

	~Territory();

	Territory& operator=(const Territory& rhs);

	bool territoryNameMatches(string territoryName);
	void addVertex(Territory* territory);
	void addEdge(MapEdge* mapEdge);
	bool isContinent();
	bool validate(int min);
	void setParent(Territory* parent);
	void setPlayerOwnership(int playerId);
	string getTerritoryName();
	TerritoryType getTerritoryType();
	string getContinentName();

private:
	// defines type
	TerritoryType* territoryType;
	// vertices is subgraph. . . . will only apply if territory is a continent, or if country definition is expanded to contain regions
	vector<Territory*>* vertices;
	// connections is other nodes this node is connected to
	vector<MapEdge*>* connections;

	/*
	* Territory's individual characteristics
	*/
	string* territoryName;
	// parent is applicable when territory is country, parent will be continent containing the country
	Territory* parent;
	// playerId is used to indicate ownership of territory by a player, applicable to countries
	int* ownerId;
};






/**
*
* Map class is the object that holds information about continents
*/
class Map {
public:
	Map(string mapName);
	Map(const Map& source);

	~Map();

	Map& operator=(const Map& rhs);

	bool territoryExists(string territoryName);
	void addTerritoryByName(const TerritoryType& territoryType, string territoryName);
	void addTerritoryByReference(Territory* territory);
	void addCountryByName(string continentName, string territoryName);
	void addCountryByReference(Territory* continent, Territory* country);
	void addEdgeByName(string territoryNameOne, string territoryNameTwo);
	void addEdgeByReference(Territory* territoryOne, Territory* territoryTwo);
	void addContinentByName(string continentName);
	void addContinentByReference(Territory* continent);
	void addVertex(Territory* territory);
	bool validate();
	void setPlayerOwnership(int playerId, string territoryName);

private:
	string* mapName;
	vector<Territory*>* vertices;
	vector<MapEdge*>* edges;
	vector<Territory*>* mapTerritories;

	int findTerritory(string territoryName);
};