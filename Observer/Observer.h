#pragma once

#include <string>
#include <vector>


#include "../Map/Map.h"
#include "../GameEngine/GameEngine.h"


using namespace std;


class Observer {
public:
	~Observer();

	virtual void update();
	
protected:
	Observer();
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

	~GameStatsObserver();

	void update();
};