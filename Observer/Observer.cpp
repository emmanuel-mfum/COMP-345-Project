
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iterator>
#include <sstream>
#include <iomanip>

#include "Observer.h"
#include "../Map/Map.h"


using namespace std;

/* ========================================================================================================= */
/*
*
* Observer
*
*/
Observer::Observer() {
	this->isActive = true;
}

void Observer::turnOff() {
	this->isActive = false;
}

string Observer::formatForOut(float toFormat) {
	stringstream ss;
	ss << fixed << setprecision(2) << toFormat;
	return ss.str();
}


/* ========================================================================================================= */
/*
*
* GameStatsObserver
*
*/
GameStatsObserver::GameStatsObserver(): Observer() {
	this->observedMap = nullptr;
}

GameStatsObserver::GameStatsObserver(Map* gameMap) : Observer() {
	this->observedMap = gameMap;
}

void GameStatsObserver::update() {
	map<int, int> countriesByPlayer;
	for (Country* country : this->observedMap->getCountries()) {
		if (country->getPlayerOwnership() != -1) {
			// country is owned
			if (countriesByPlayer.find(country->getPlayerOwnership()) == countriesByPlayer.end()) {
				// not found
				// new entry
				countriesByPlayer.insert(pair<int, int>(country->getPlayerOwnership(), 1));
			}
			else {
				// player exists
				// increment
				countriesByPlayer[country->getPlayerOwnership()]++;
			}
		}
	}

	cout << "\n======================================================================\n";
	cout << "Observer: GameStatsObserver";
	cout << "\n  Continents: " + to_string(this->observedMap->getNumContinents());
	cout << "\n  Countries: " + to_string(this->observedMap->getNumCountries());
	cout << "\n";
	for (map<int, int>::iterator itr = countriesByPlayer.begin(); itr != countriesByPlayer.end(); ++itr) {
		float playerPct = ((float)(itr->second)) / ((float) this->observedMap->getNumCountries());
		cout << "\n  Player " + to_string(itr->first) + " controls " + this->formatForOut(playerPct) + "%, " + to_string(itr->second) + " of " + to_string(this->observedMap->getNumCountries()) + " countries";
	}
	cout << "\n======================================================================\n\n";
}

void GameStatsObserver::registerMap(Map* gameMap) {
	this->observedMap = gameMap;
	
}

/* ========================================================================================================= */
/*
*
* PhaseObserver
*
*/