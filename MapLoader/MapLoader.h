#include <iostream>

class Map;

class MapLoader {
// need to add colon after public
public:
	static Map load_map(std::string fName);
};