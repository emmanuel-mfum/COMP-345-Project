
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iterator>
#include <sstream>
#include <iomanip>

#include "Observer.h"
#include "../Map/Map.h"
#include "../Order/Orders.h"


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
	this->currentNumPlayers = 0;
	this->eliminated;
}

GameStatsObserver::GameStatsObserver(Map* gameMap) : Observer() {
	this->observedMap = gameMap;
	this->currentNumPlayers = 0;
	this->eliminated;
}

GameStatsObserver::~GameStatsObserver() {
	this->observedMap = nullptr;
}

void GameStatsObserver::update() {
	if (!this->isActive) {
		return;
	}

	int numPlayers = 0;
	int numNeutralCountries = 0;
	map<int, int> countriesByPlayer;
	for (Country* country : this->observedMap->getCountries()) {
		if (country->getPlayerOwnership() != -1) {
			// country is owned
			if (countriesByPlayer.find(country->getPlayerOwnership()) == countriesByPlayer.end()) {
				// not found
				// new entry
				countriesByPlayer.insert(pair<int, int>(country->getPlayerOwnership(), 1));
				numPlayers++;
			}
			else {
				// player exists
				// increment
				countriesByPlayer[country->getPlayerOwnership()]++;
			}
		}
		else {
			numNeutralCountries++;
		}
	}
	int winnerId = 0;
	int winnerNum = 0;
	int count = 0;

	cout << "\n======================================================================\n";
	cout << "Observer: GameStatsObserver";
	cout << "\n  Continents: " + to_string(this->observedMap->getNumContinents());
	cout << "\n  Countries: " + to_string(this->observedMap->getNumCountries());
	cout << "\n";
	
	for (map<int, int>::iterator itr = countriesByPlayer.begin(); itr != countriesByPlayer.end(); ++itr) {
		float playerPct = ((float)(itr->second)) / ((float) this->observedMap->getNumCountries()) * 100;
		cout << "\n  Player " + to_string(itr->first) + " controls " + this->formatForOut(playerPct) + "%, " + to_string(itr->second) + " of " + to_string(this->observedMap->getNumCountries()) + " countries";
		count++;
		winnerId = itr->first;
		winnerNum = itr->second;
	}

	if (numNeutralCountries > 0) {
		cout << "\n\n  There are currently " + to_string(numNeutralCountries) + " neutral countries";
	}

	int totalPlayers = this->currentNumPlayers + this->eliminated.size();

	if (numPlayers < this->currentNumPlayers ) {
		int counter = 0;
		int idx = 0;
		cout << "\n";
		while (counter < (currentNumPlayers - numPlayers) && idx < totalPlayers) {
			if (countriesByPlayer.find(idx) == countriesByPlayer.end()) {
				for (int i : this->eliminated) {
					if (i != idx) {
						cout << "\n  Player " + to_string(idx) + " was eliminated";
						this->eliminated.push_back(idx);
						counter++;
					}
				}
			}
			idx++;
		}
	}
	if (count == 1 && this->observedMap->getNumCountries() == winnerNum) {
		cout << "\n\n  *****************************************";
		cout <<   "\n  Player " + to_string(winnerId) + " has won the game!";
		cout <<   "\n  CONGRATULATIONS!!!";
		cout <<   "\n  *****************************************\n";
	}

	cout << "\n======================================================================\n\n" << flush;

	this->currentNumPlayers = numPlayers;
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
	this->roundOrders;
	this->roundOrderInfo;
}

PhaseObserver::~PhaseObserver() {
	this->engine = nullptr;
}

void PhaseObserver::addExecutedOrder(Order* order) {
	if (order != nullptr) {
		if (this->roundOrderInfo.find(order->getPlayer()->getPlayerId()) == this->roundOrderInfo.end()) {
			this->roundOrderInfo.insert(pair<int, map<string, int>>(order->getPlayer()->getPlayerId(), map<string, int>()));
		}
		if (this->roundOrderInfo[order->getPlayer()->getPlayerId()].find(order->getType()) == this->roundOrderInfo[order->getPlayer()->getPlayerId()].end()) {
			this->roundOrderInfo[order->getPlayer()->getPlayerId()].insert(pair<string, int>(order->getType(), 0));
		}

		this->roundOrderInfo[order->getPlayer()->getPlayerId()][order->getType()] += order->getNum();

		this->roundOrders.push_back(order->getStringDescription());
	}
	else {
		this->roundOrders.push_back("\n\n");
	}
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
				cout << "\n================================================================================" << endl;
				cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n" << endl;
				cout << "PhaseObserver:: ORDERS EXECUTED\n\n" << endl;

				cout << "   Orders in played order::\n\n" << endl;
				for (string orderString : this->roundOrders) {
					cout << orderString << endl;
				}

				cout << "\n\n   Overview by player::\n\n" << endl;

				for (auto itr = this->roundOrderInfo.begin(); itr != this->roundOrderInfo.end(); ++itr) {
					cout << "      Player " + to_string(itr->first) + ":" << endl;
					for (auto ordItr = itr->second.begin(); ordItr != itr->second.end(); ++ordItr) {
						if (ordItr->first.compare("Deploy") == 0) {
							cout << "        Deployed " + to_string(ordItr->second) + " armies" << endl;
						}
						else if (ordItr->first.compare("Advance") == 0) {
							cout << "        Advanced " + to_string(ordItr->second) + " armies" << endl;
						}
						else if (ordItr->first.compare("Bomb") == 0) {
							cout << "        Bombed opponent Player " + to_string(ordItr->second) << endl;
						}
						else if (ordItr->first.compare("Blockade") == 0) {
							cout << "        Performed blockade, resulting in " + to_string(ordItr->second) + " free armies" << endl;
						}
						else if (ordItr->first.compare("Negotiate") == 0) {
							cout << "        Engaged in diplomacy with Player " + to_string(ordItr->second) << endl;
						}
						else {
							cout << "        Airlifted " + to_string(ordItr->second) + " armies" << endl;
						}
					}
				}

				cout << "\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^" << endl;
				cout << "================================================================================\n" << endl;
				this->roundOrderInfo.clear();
				this->roundOrders.clear();
			}
		}
	}
}