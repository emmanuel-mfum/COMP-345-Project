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

	bool startPlayerExecute;
	int currentPlayerIdx;
	int currentPlayerReinf;
	vector<bool> eliminated;
	int currentlyCreatingPlayer;
	
	GameStatsObserver* gsObs;
	PhaseObserver* phaseObs;

	static string mapSelection(int choice);
	static int numberOfPlayers();
	static bool ObserverOption(string s);

	Player* createPlayer();

public:
	
	static  string directory;
	GameEngine();
	GameEngine(const GameEngine& gameEngine);
	~GameEngine();
	void gameStart();
	void startupPhase();
	void mainGameLoop();

	Phases getCurrentPhase();
	int getPlayerAtCurIdx();
	int getCurrentReinf();
	bool startingPlayerExecute() { return this->startPlayerExecute; }

	void reinforcementPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();

	void smallGameStart(Map* map, vector<Player*> playerOrder);
};
