#pragma once
#include <iostream>
#include <string>

#include "../Map/Map.h"

class Map;

class MapLoader {

private: 
	Map* newMap;

public:
	MapLoader();
	MapLoader(string fName);
	~MapLoader();
	static Map* load_map(std::string fName);
};
