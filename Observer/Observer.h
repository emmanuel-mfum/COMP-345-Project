#pragma once

#include <string>
#include <vector>
#include <map>


#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"
#include "../Order/Orders.h"


using namespace std;

class Map;
class Observer;
class PhaseObserver;
class GameStatsObserver;
class GameEngine;
enum class Phases;
class Order;


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

	void addExecutedOrder(Order* order);

private:
	GameEngine* engine;
	Phases currentEnginePhase;
	map<int, map<string, int>> roundOrderInfo;
	vector<string> roundOrders;
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
	int currentNumPlayers;
	vector<int> eliminated;
};