#pragma once

#include "MapLoader.h"
#include "../Map/Map.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


using namespace std;

//default constructor
MapLoader::MapLoader() {
	this->newMap = nullptr;
}
//parameterized constructor
MapLoader::MapLoader(string fName) {
	this->newMap = load_map(fName);
}

//destructor
MapLoader::~MapLoader() {
	delete newMap;
}


Map* MapLoader::load_map(string fName) {
	Map* map = nullptr;
	
	std::ifstream input_stream(fName);
	std::string line_read;

	bool isContinents = false;
	bool isCountries = false;
	bool isBorders = false;

	bool readSplitToken = false;
	
	vector<string>* continents = new vector<string>;
	vector<string>* countries = new vector<string>;

	string delimiter = " ";

	// Load the continents.
	int counter = 0;
	while (getline(input_stream, line_read)) {
		string line = line_read;
		readSplitToken = false;

		// assume continents comes first, countries comes after, borders come after
		if (counter == 0) {
			// create a new map with the name
			string mapName = line.substr(line.find("map: ") + 5);
			map = new Map(mapName);
		}
		else if (line.compare("[continents]") == 0) {
			// have to start reading continents
			cout << "READING CONTINENTS\n" << endl;
			isContinents = true;
			readSplitToken = true;
		}
		else if (line.compare("[countries]") == 0) {
			// connect all the continents once we have finished reading them all
			cout << "\n\nCONNECTING CONTINENTS\n" << endl;
			for (int i = 0; i < continents->size() - 1; i++) {
				cout << "Connected: " << continents->at(i) << " --> " << continents->at(i + 1) << endl;
				map->addEdgeByName(continents->at(i), continents->at(i + 1));
			}
			cout << "\n\nREADING CONTINENTS\n" << endl;
			// have to start reading countries
			isContinents = false;
			isCountries = true;
			readSplitToken = true;
		}
		else if (line.compare("[borders]") == 0) {
			cout << "\n\nCONNECTING COUNTRIES\n" << endl;
			// have to start reading the borders
			isCountries = false;
			isBorders = true;
			readSplitToken = true;
		}

		if (!readSplitToken && line.compare("") != 0) {
			// have to read the component on the line
			if (isContinents) {
				// add the continent to the map
				string continentName = line.substr(0, line.find(" "));
				map->addContinentByName(continentName);
				cout << "Created territory: " + continentName << endl;
				continents->push_back(continentName);
			}
			else if (isCountries) {
				// add the country to the map
				size_t pos = 0;
				string token;
				int countryCounter = 0;
				string countryName;
				// tokenize the line
				while ((pos = line.find(delimiter)) != string::npos) {
					token = line.substr(0, pos);
					// found the country name
					if (countryCounter == 1) {
						countryName = token;
					}
					// now know which continent the country belongs to, can be added to map
					else if (countryCounter == 2) {
						string continentName = continents->at(stoi(token) - 1);
						map->addCountryByName(continentName, countryName);
						cout << "Created territory: " + countryName << endl;
						countries->push_back(countryName);
					}

					line.erase(0, pos + delimiter.length());
					countryCounter++;
				}
			}
			else if (isBorders) {
				// add the border to the map
				size_t pos = 0;
				string token;
				string countryName;
				int borderCounter = 0;
				// tokenize the string
				while ((pos = line.find(delimiter)) != string::npos) {
					token = line.substr(0, pos);
					// found which country the borders are for
					if (borderCounter == 0) {
						countryName = countries->at(stoi(token) - 1);
					}

					else {
						// add an edge
						cout << "Connected: " << countryName << " --> " << countries->at(stoi(token) - 1) << endl;
						map->addEdgeByName(countryName, countries->at(stoi(token) - 1));
					}

					line.erase(0, pos + delimiter.length());
					borderCounter++;
				}
			}
		}
		counter++;
	}

	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);

	bool isValid = map->validate();
	return map;
}

