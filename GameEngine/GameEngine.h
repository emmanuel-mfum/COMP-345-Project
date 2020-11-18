#include "Map.h"
#include "MapLoader.h"
#include "Player.h"
#include "Cards.h"
#include <string>
#include <iostream>


class Player;
class Deck;
class Map;


class GameEngine
{
private :
	list <Player> playerList;
	Player* player;
	Map* map;
	Deck* deck;
	bool phaseObserverOption;
	bool StatisticsObserverOption;
	static string mapSelection();
	static int numberOfPlayers();
	static bool ObserverOption();
public:
	
	static const char* directory;
	GameEngine();
	~GameEngine();
	void gameStart();
	void startupPhase();

};
