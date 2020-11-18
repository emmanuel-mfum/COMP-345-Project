#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/player.h"
#include "../Card/Cards.h"
#include <string>
#include <iostream>


class Player;
class Deck;
class Map;

enum class Phases {
	NIL = 0,
	Reinforcement = 1,
	IssueOrder = 2,
	ExecuteOrder = 3
};


class GameEngine
{
private :
	vector<Player*> playerList;
	vector<Player*> playerOrder;
	Player* player;
	Map* map;
	Deck* deck;
	Phases currentPhase;
	bool phaseObserverOption;
	bool StatisticsObserverOption;
	static string mapSelection();
	static int numberOfPlayers();
	static bool ObserverOption(string s);

	void reinforcementPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();
public:
	
	static const char* directory;
	GameEngine();
	~GameEngine();
	void gameStart();
	void startupPhase();
	void mainGameLoop();
};
