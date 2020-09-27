#include <string>
#include "Map.h"


using namespace std;

Territory::Territory(const TerritoryType &territoryType, string territoryName) {
	this->territoryType = territoryType;
	this->territoryName = &territoryName.copy();
}


TerritoryType