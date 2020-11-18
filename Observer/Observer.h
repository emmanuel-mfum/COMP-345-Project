#pragma once

#include <string>
#include <vector>


#include "../Map/Map.h"


using namespace std;

class Map;
class Observer;
class PhaseObserver;
class GameStatsObserver;


class Observer {
public:
	Observer();
	virtual void update() = 0;
	void turnOff();
	
protected:
	
	string formatForOut(float toFormat);

private:
	bool isActive;
	
};


class PhaseObserver : public Observer {
public:
	PhaseObserver();

	~PhaseObserver();

	void update();

private:

	
};


class GameStatsObserver : public Observer {
public:
	GameStatsObserver();
	GameStatsObserver(Map* gameMap);

	void update();
	void registerMap(Map* map);

private:
	Map* observedMap;
};