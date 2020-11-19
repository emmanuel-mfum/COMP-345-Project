
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

Observer::~Observer() {

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

GameStatsObserver::~GameStatsObserver() {
	this->observedMap = nullptr;
}

void GameStatsObserver::update() {
	if (!this->isActive) {
		return;
	}

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
		float playerPct = ((float)(itr->second)) / ((float) this->observedMap->getNumCountries()) * 100;
		cout << "\n  Player " + to_string(itr->first) + " controls " + this->formatForOut(playerPct) + "%, " + to_string(itr->second) + " of " + to_string(this->observedMap->getNumCountries()) + " countries";
	}
	cout << "\n======================================================================\n\n" << flush;
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
PhaseObserver::PhaseObserver(GameEngine* engine) {
	this->engine = engine;
	this->currentEnginePhase = Phases::NIL;
}

PhaseObserver::~PhaseObserver() {
	this->engine = nullptr;
}

void PhaseObserver::update() {
	if (!this->isActive) {
		return;
	}

	if (this->currentEnginePhase != this->engine->getCurrentPhase()) {
		cout << "\nPhaseObserver: Game moving from " << this->currentEnginePhase << " to " << this->engine->getCurrentPhase() << "\n" << endl;
		this->currentEnginePhase = this->engine->getCurrentPhase();
	} 
	else {
		if (this->currentEnginePhase == Phases::Reinforcement) {
			cout << "\nPhaseObserver: Player " << this->engine->getPlayerAtCurIdx() << " reinforced with " << this->engine->getCurrentReinf() << "\n" << endl;
		}
		else if (this->currentEnginePhase == Phases::IssueOrder) {
			cout << "\nPhaseObserver: Player " << this->engine->getPlayerAtCurIdx() << " issuing orders...\n" << endl;
		}
		else if (this->currentEnginePhase == Phases::ExecuteOrder) {
			if (this->engine->startingPlayerExecute()) {
				cout << "\nPhaseObserver: Player " << this->engine->getPlayerAtCurIdx() << " starting to execute orders...\n" << endl;
			}
			else {
				cout << "\nPhaseObserver: Player " << this->engine->getPlayerAtCurIdx() << " executing `An Order`\n" << endl;
			}
		}
	}
}