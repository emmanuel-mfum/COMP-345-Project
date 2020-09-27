#pragma once

#include <string>
using namespace std;
/**
*
* This file contains Map, Territory, Country, Continent, and MapEdge classes
*
*/





/**
*
* Map class is the object that holds information about continents
*/
class Map {
public:
	Map() {

	}

	~Map() {

	}

private:
	static int* mapId;

	string* mapName;
	Territory* vertices;
	MapEdge* edges;
}


/*
*
* Territory is the class that represents a vertex in the graph
*/
class Territory {
public:
	Territory(const TerritoryType &territoryType, string territoryName) {}

	~Territory() {

	}

private:
	// defines type
	TerritoryType* territoryType;
	// vertices is subgraph. . . . will only apply if territory is a continent, or if country definition is expanded to contain regions
	Territory* vertices;
	// connections is other nodes this node is connected to
	Territory* connections;

	/*
	* Territory's individual characteristics
	*/
	string* territoryName;
}



enum TerritoryType {
	Country=1,
	Continent=2,
};




/*
*
* MapEdge represents connection between two MapVertices
*/
class MapEdge {
public:
	MapEdge() {

	}

	~MapEdge() {

	}
}