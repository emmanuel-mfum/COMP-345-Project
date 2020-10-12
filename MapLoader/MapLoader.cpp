#include "MapLoader.h"
#include "../Map/Map.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

//default constructor
MapLoader::MapLoader() {
	newMap = new Map();
}
//parameterized constructor
MapLoader::MapLoader(string fName) {
	Map* newMap = load_map(fName);
}

//destructor
MapLoader::~MapLoader() {
	delete newMap;
}


Map MapLoader::load_map(string fName) {
	Map* map = nullptr;
	
	std::ifstream input_stream(fName);
	std::string line_read;

	// Load the continents.
	while (getline(input_stream, line_read) && line_read != "[continents]") {}
	while (getline(input_stream, line_read) && line_read != "") {
		//Set all continents      
		 const std::string newContinentName = line.substr();
		 map->addContinentByName(newContinentName);

	}

	// Load the countries
	while (getline(input_stream, line_read) && line_read != "[countries]") {}
	while (getline(input_stream, line_read)) {
		//Set all countries
		Territory* newCountry = new Territory(TerritoryType::Country, "newCountryName");
		map->addContinentByName(continentName, newCountryName);
		map->addEdgeByName(territoryName, territoryName);
	}
	// The input stream is restarted from the beginning.
	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);


	//Set territories

	return map;

}

