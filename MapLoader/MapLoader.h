#pragma once
#include <iostream>
#include <string>

#include "../Map/Map.h"

using namespace std;

class Map;


class MapLoader {

private: 
	Map* newMap;

public:
	MapLoader(string fName);
	~MapLoader();
	static Map* load_map(std::string fName);
}

class ConquestFileReader
{
private:
	Map* newConquestMap
public:
	ConquestFileReader(string fName);
	~ConquestFileReader();
	static Map* load_ConquestMap(std::string fName);
};

class ConquestFileReaderAdapter : public MapLoader, public ConquestFileReader {

private:
	MapLoader* domination;
	ConquestFileReader* conquest;
	bool isDomination;

public:
	ConquestFileReaderAdapter(MapLoader* maploader);
	ConquestFileReaderAdapter(ConquestFileReader* conquest);
	void setBool(int input);
	Map* parseMap(string name);


};
