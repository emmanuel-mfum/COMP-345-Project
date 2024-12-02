#pragma once

#include "MapLoader.h"
#include "../Map/Map.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>


using namespace std;


MapLoader::MapLoader() {}

//parameterized constructor

MapLoader::MapLoader(string fName) {
	this->filename = fName;
	this->newMap = load_map(fName);
}

//destructor
MapLoader::~MapLoader() {
	delete newMap;
}

//load map for domination
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
	cout << "MAPLOADER:: loading map - " << fName << "\n\n" << endl;
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
			cout << "   READING CONTINENTS\n" << endl;
			isContinents = true;
			readSplitToken = true;
		}
		else if (line.compare("[countries]") == 0) {
			// connect all the continents once we have finished reading them all
			cout << "\n\n   CONNECTING CONTINENTS\n" << endl;
			for (int i = 0; i < continents->size() - 1; i++) {
				// cout << "Connected: " << continents->at(i) << " --> " << continents->at(i + 1) << endl;
				map->addEdgeByName(continents->at(i), continents->at(i + 1));
			}
			cout << "\n\n   READING CONTINENTS\n" << endl;
			// have to start reading countries
			isContinents = false;
			isCountries = true;
			readSplitToken = true;
		}
		else if (line.compare("[borders]") == 0) {
			cout << "\n\n   CONNECTING COUNTRIES\n" << endl;
			// have to start reading the borders
			isCountries = false;
			isBorders = true;
			readSplitToken = true;
		}

		if (!readSplitToken && line.compare("") != 0) {
			// have to read the component on the line
			if (isContinents) {
				// add the country to the map
				size_t pos = 0;
				string token;
				int continentCounter = 0;
				string continentName;
				while ((pos = line.find(delimiter)) != string::npos) {
					token = line.substr(0, pos);
					// add the continent to the map
					if (continentCounter == 0) {
						continentName = token;
						map->addContinentByName(continentName);
						// cout << "Created territory: " + continentName << endl;
						continents->push_back(continentName);
					}
					else if (continentCounter == 1) {
						int continentBonus = stoi(token);
						map->setContinentBonus(continentName, continentBonus);
					}

					line.erase(0, pos + delimiter.length());
					continentCounter++;
				}
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
						// cout << "Created territory: " + countryName << endl;
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
						// cout << "Connected: " << countryName << " --> " << countries->at(stoi(token) - 1) << endl;
						map->addEdgeByName(countryName, countries->at(stoi(token) - 1));
					}

					line.erase(0, pos + delimiter.length());
					borderCounter++;
				}
				try {
					// cout << "Connected: " << countryName << " --> " << countries->at(stoi(line) - 1) << endl;
					map->addEdgeByName(countryName, countries->at(stoi(line) - 1));
				}
				catch (const exception& e) {
				}

			}
		}
		counter++;
	}

	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);

	if (map->validate()) {
		cout << "   Map was valid!!" << endl;
	}
	else {
		cout << "   Map was invalid, aborting. . ." << endl;
		throw exception("Invalid map");
	}

	return map;
}

//load map for conquest


string MapLoader::getFileName()
{
	return this->filename;
}



Map* ConquestFileReader::load_ConquestMap(string fName) {
	Map* ConquestMap = nullptr;

	std::ifstream input_stream(fName);
	std::string line_read;


	bool isContinents = false;
	bool isTerritories = false;
	bool isBorders = false;
	bool readSplitToken = false;
	

	vector<string>* continents = new vector<string>;
	vector<string>* territories = new vector<string>;
	cout << "ConquestFileReader::loading ConquestMap " << fName << "\n\n" << endl;
	string delimiter = " ";

	// Load the continents.
	int counter = 0;
	while (getline(input_stream, line_read)) {
		string line = line_read;
		readSplitToken = false;

		// assume continents comes first, territories comes after
		if (counter == 0) {
			// create a new map with the name
			string mapName = line.substr(line.find("map: ") + 5);
			ConquestMap = new Map(mapName);
		}
		else if (line.compare("[continents]") == 0) {
			// have to start reading continents
			cout << "   READING CONTINENTS\n" << endl;
			isContinents = true;
			readSplitToken = true;
		}
		else if (line.compare("[territories]") == 0) {
			// connect all the continents once we have finished reading them all
			cout << "\n\n   CONNECTING CONTINENTS\n" << endl;
			for (int i = 0; i < continents->size() - 1; i++) {
				// cout << "Connected: " << continents->at(i) << " --> " << continents->at(i + 1) << endl;
				ConquestMap->addEdgeByName(continents->at(i), continents->at(i + 1));
			}
			cout << "\n\n   READING CONTINENTS\n" << endl;
			// have to start reading countries
			isContinents = false;
			isTerritories = true;
			readSplitToken = true;
		}
		else if (line.compare("[borders]") == 0) {
			cout << "\n\n   CONNECTING COUNTRIES\n" << endl;
			// have to start reading the borders
			isTerritories = false;
			isBorders = true;
			readSplitToken = true;
		}

		if (!readSplitToken && line.compare("") != 0) {
			// have to read the component on the line
			if (isContinents) {
				// add the country to the map
				size_t pos = 0;
				string token;
				int continentCounter = 0;
				string continentName;
				while ((pos = line.find(delimiter)) != string::npos) {
					token = line.substr(0, pos);
					// add the continent to the map
					if (continentCounter == 0) {
						continentName = token;
						ConquestMap->addContinentByName(continentName);
						// cout << "Created territory: " + continentName << endl;
						continents->push_back(continentName);
					}
					else if (continentCounter == 1) {
						int continentBonus = stoi(token);
						ConquestMap->setContinentBonus(continentName, continentBonus);
					}

					line.erase(0, pos + delimiter.length());
					continentCounter++;
				}
			}
			else if (isTerritories) {
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
						ConquestMap->addCountryByName(continentName, countryName);
						// cout << "Created territory: " + countryName << endl;
						territories->push_back(countryName);
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
						countryName = territories->at(stoi(token) - 1);
					}

					else {
						// add an edge
						// cout << "Connected: " << countryName << " --> " << countries->at(stoi(token) - 1) << endl;
						ConquestMap->addEdgeByName(countryName, territories->at(stoi(token) - 1));
					}

					line.erase(0, pos + delimiter.length());
					borderCounter++;
				}
				try {
					// cout << "Connected: " << countryName << " --> " << countries->at(stoi(line) - 1) << endl;
					ConquestMap->addEdgeByName(countryName, territories->at(stoi(line) - 1));
				}
				catch (const exception& e) {
				}

			}
		}
		counter++;
	}

	input_stream.clear();
	input_stream.seekg(0, input_stream.beg);

	return ConquestMap;
}


//parameterized constructor for conquestMap
ConquestFileReader::ConquestFileReader(string fName) {
	this->newConquestMap=load_ConquestMap(fName);

}

ConquestFileReader::ConquestFileReader() {}

//destructor for conquestMap
ConquestFileReader::~ConquestFileReader() {
	delete newConquestMap;
}


// default constructor for ConquestFileReaderAdapter
ConquestFileReaderAdapter::ConquestFileReaderAdapter()
{
	
}

// Parameterized constructor for ConquestFileReaderAdapter
ConquestFileReaderAdapter::ConquestFileReaderAdapter(MapLoader* maploader)
{
	domination = maploader;
	conquest = NULL;
	isDomination = true;
}

// Parameterized constructor for ConquestFileReaderAdapter
ConquestFileReaderAdapter::ConquestFileReaderAdapter(ConquestFileReader* conquest)
{
	conquest = conquest;
	domination = NULL;
	isDomination = false;
}

// destructor for ConquestFileReaderAdapter
ConquestFileReaderAdapter::~ConquestFileReaderAdapter(){
	delete domination;
	domination = nullptr;
	delete conquest;
	conquest = nullptr;

}

// Copy constructor for ConquestFileReaderAdapter
ConquestFileReaderAdapter::ConquestFileReaderAdapter(ConquestFileReaderAdapter& mapReader) {
	domination = mapReader.domination;
	conquest = mapReader.conquest;
	isDomination = mapReader.isDomination;
}

// Assignment operator for ConquestFileReaderAdapter
ConquestFileReaderAdapter& ConquestFileReaderAdapter::operator=(const ConquestFileReaderAdapter& mapR)
{
	this->domination = mapR.domination;
	this->conquest = mapR.conquest;
	this->isDomination = mapR.isDomination;
	return *this;
}

// Stream operator for ConquestFileReaderAdapter
ostream& operator<<(ostream& out, const ConquestFileReaderAdapter& mapR) {
	out << "The name of the file currently used: " << endl;
	if (!mapR.fileName.compare("")) {
		out << "File doesn't exist !!! " << endl;
	}
	else {
		out << mapR.fileName << endl;
	}
	return out;
}

// setter for isDomination 
 void ConquestFileReaderAdapter::setBool(int input) {

	if (input == 0) {

		this->isDomination = true; // the user wants a domination map
	
	}
	else {
		this->isDomination = false; // the user wants a conquest map
	}

}
 // method to load the map according to the type specified by the user
Map* ConquestFileReaderAdapter::parseMap(string name) {

	if (isDomination) {

		
		// parse the map as a we did it before
		return this->domination->load_map("../Map_Directory/" + name);
	
	
	}
	else {
		// use the methods in ConquestFileReader
		return this->conquest->load_ConquestMap("../Map_Directory2/" + name);
	}
}

// getter for the file name
string ConquestFileReaderAdapter::getFileName() {

	return this->fileName;
}

// setter for the file name
void ConquestFileReaderAdapter::setFileName(string name) {

	this->fileName = name;
}
