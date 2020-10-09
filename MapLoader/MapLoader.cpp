#include "MapLoader.h"
#include "Map.h"
#include <iostream>
#include <sstream>
#include <fstream>



using namespace std;

Map MapLoader::load_map(std::string fName) {
	Map map;

	std::ifstream input_stream(fName);
	std::string line_read;

	// Load the continents.
	while (std::getline(input_stream, line_read) && line_read != "[continents]") {}
	while (std::getline(input_stream, line_read) && line_read != "") {
		//set all continents
		//check if it's digit - if it is 
		//check the color
	}

	// Load the countries
	while (std::getline(input_stream, line_read) && line_read != "[countries]") {}
	while (std::getline(input_stream, line_read)) {
		//set all countries
	}

	// The input stream is restarted from the beginning.
	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);




	//Set the borders

}