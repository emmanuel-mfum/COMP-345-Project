#pragma once

#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/player.h"
#include "../Card/Cards.h"
#include "../Observer/Observer.h"
#include <string>
#include <iostream>


class Player;
class Deck;
class Map;
class GameStatsObserver;
class PhaseObserver;

enum class Phases {
	NIL = 0,
	Reinforcement = 1,
	IssueOrder = 2,
	ExecuteOrder = 3
	
};

ostream& operator<<(ostream& out, Phases e);


class GameEngine
{
private :
	vector<Player*> playerList;
	vector<Player*> playerOrder;
	Map* map;
	Deck* deck;
	Phases currentPhase;

	int currentPlayerIdx;
	int currentPlayerReinf;
	
	GameStatsObserver* gsObs;
	PhaseObserver* phaseObs;

	static string mapSelection();
	static int numberOfPlayers();
	static bool ObserverOption(string s);

	void reinforcementPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();
public:
	
	static const char* directory;
	GameEngine();
	GameEngine(const GameEngine& gameEngine);
	~GameEngine();
	void gameStart();
	void startupPhase();
	void mainGameLoop();

	Phases getCurrentPhase();
	int getPlayerAtCurIdx();
	int getCurrentReinf();
};
