#include "Map.h"
#include "MapLoader.h"
#include "Player.h"
#include "Cards.h"

#include <list>


class Player;
class Deck;
class Map;
class Continent;


class GameEngine
{
private :

	list <Player> playerList;
	Player* Player;
	Map* map;
	Deck* deck;

public:
	
	static const char* directory;

	GameEngine();
	~GameEngine();

	static void gameStart();
	static string mapSelection();
	static int playerSelection();
	bool ObserverFunction();


	
	
};
