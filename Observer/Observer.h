#pragma once

#include <string>
#include <vector>


#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"


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
	virtual ~Observer();
	
protected:
	
	string formatForOut(float toFormat);

protected:
	bool isActive;
	
};


class PhaseObserver : public Observer {
public:
	PhaseObserver(GameEngine* engine);

	~PhaseObserver();

	void update();

private:
	GameEngine* engine;
	Phases currentEnginePhase;
};


class GameStatsObserver : public Observer {
public:
	GameStatsObserver();
	GameStatsObserver(Map* gameMap);

	~GameStatsObserver();

	void update();
	void registerMap(Map* map);

private:
	Map* observedMap;
};