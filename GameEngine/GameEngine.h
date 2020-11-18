#include "../Map/Map.h"
#include "../MapLoader/MapLoader.h"
#include "../Player/player.h"
#include "../Card/Cards.h"
#include <string>
#include <iostream>


class Player;
class Deck;
class Map;


class GameEngine
{
private :
	vector<Player*> playerList;
	vector<Player*> playerOrder;
	Player* player;
	Map* map;
	Deck* deck;
	bool phaseObserverOption;
	bool StatisticsObserverOption;
	static string mapSelection();
	static int numberOfPlayers();
	static bool ObserverOption(string s);
public:
	
	static const char* directory;
	GameEngine();
	~GameEngine();
	void gameStart();
	void startupPhase();

};
