#include "MapLoader.h"
#include "../Map/Map.h"
#include <iostream>
#include <sstream>
#include <fstream>



using namespace std;

Map MapLoader::load_map(std::string fName) {
	Map* map = nullptr;

	std::ifstream input_stream(fName);
	std::string line_read;
	// get the name from the top of the file
	// instantiate the map object with a name

	// Load the continents.
	while (std::getline(input_stream, line_read) && line_read != "[continents]") {}
	while (std::getline(input_stream, line_read) && line_read != "") {
		//set all continents
		//check if it's digit - if it is 
		//check the color
		// are the colour and digit important???
		// for each continent create it like
		// map->addContinentByName(newContinentName);

	}

	// Load the countries
	while (std::getline(input_stream, line_read) && line_read != "[countries]") {}
	while (std::getline(input_stream, line_read)) {
		//set all countries
		// Territory* newCountry = new Territory(TerritoryType::Country, "newCountryName");
		// then add the country to the map like
		// map->addContinentByName(continentName, newCountryName);
		// add all the edges like
		// map->addEdgeByName(territoryName, territoryName);
	}

	// The input stream is restarted from the beginning.
	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);




	//Set the borders

}