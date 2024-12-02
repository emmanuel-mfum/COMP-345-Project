#pragma once
#include <iostream>
#include <string>

#include "../Map/Map.h"

using namespace std;

class Map;


class MapLoader {

private:
	Map* newMap;
	string filename;

public:
	MapLoader();
	MapLoader(string fName);
	~MapLoader();
	static Map* load_map(std::string fName);
	string getFileName();
};

class ConquestFileReader
{
private:
	Map* newConquestMap;
public:
	ConquestFileReader();
	ConquestFileReader(string fName);
	~ConquestFileReader();
	static Map* load_ConquestMap(std::string fName);
};



class ConquestFileReaderAdapter : public MapLoader, public ConquestFileReader {

private:
	MapLoader* domination;
	ConquestFileReader* conquest;
	bool isDomination;
	string fileName;

public:
	ConquestFileReaderAdapter();
	ConquestFileReaderAdapter(MapLoader* maploader);
	ConquestFileReaderAdapter(ConquestFileReader* conquest);
	ConquestFileReaderAdapter(ConquestFileReaderAdapter& mapReader);
	~ConquestFileReaderAdapter();
	friend ostream& operator<<(ostream& out, const ConquestFileReaderAdapter& mapL); // stream operator
	ConquestFileReaderAdapter& operator=(const ConquestFileReaderAdapter& mapL); // assignment operator
	void setBool(int input);
	Map* parseMap(string name);
	string getFileName();
	void setFileName(string fileName);


};
