#pragma once

#include <string>
#include <vector>
#include <map>

#include "../Observer/Observer.h"
#include "../Player/player.h"

using namespace std;
/**
*
* This file contains Map, MapComponent, Country, Continent, and MapEdge classes
*
*/
class Map;
class MapComponent;
class MapEdge;
class Continent;
class Country;
class GameStatsObserver;
class Player;


enum class TerritoryType {
	Country = 1,
	Continent = 2
};


/* ========================================================================================================= */
/*
* 
* 
* MAP EDGE
* 
* 
*/
class MapEdge {
public:
	MapEdge(MapComponent* territoryOne, MapComponent* territoryTwo);
	MapEdge(const MapEdge& source);

	~MapEdge();

	MapEdge& operator=(const MapEdge& rhs);

	friend std::ostream& operator<<(std::ostream& out, const MapEdge& toOut);

	MapComponent* getAdjacentTerritory(string territoryName);
	bool containsBothTerritories(MapComponent* territoryOne, MapComponent* territoryTwo);

private:
	MapComponent* territoryOne;
	MapComponent* territoryTwo;
};


/* ========================================================================================================= */
/*
* 
* 
* MAP COMPONENT
* 
* 
*/
class MapComponent {
public:
	MapComponent(string territoryName, TerritoryType t);
	MapComponent(const MapComponent& source);

	virtual ~MapComponent();

	virtual MapComponent& operator=(const MapComponent& rhs);

	friend ostream& operator<<(ostream& out, const MapComponent& toOut);

	bool territoryNameMatches(string territoryName);
	void addEdge(MapEdge* edge);
	bool validate(int min);
	string getTerritoryName();
	vector<MapComponent*> getAdjacentTerritories();
	vector<Country*> getAdjacentCountries();
	TerritoryType getTerritoryType();
	int getId();
	

private:
	static int getAndUpdateIdForNew();
	static int componentCounter;

protected:
	TerritoryType territoryType;
	int territoryId;
	string territoryName;
	vector<MapEdge*> connections;
};



/* ========================================================================================================= */
/*
*
*
* CONTINENT
*
*
*/
class Continent : public MapComponent {
public:
	Continent(string territoryName);
	Continent(const Continent& source);
	
	~Continent();

	Continent& operator=(const Continent& rhs);

	friend ostream& operator<<(ostream& out, const Continent& toOut);

	void addVertex(Country* country);
	int getNumCountries();
	void setBonus(int bonus) { this->continentBonus = bonus; }
	int getBonus() { return this->continentBonus; }

private:
	vector<Country*> vertices;
	int continentBonus;
};



/* ========================================================================================================= */
/*
*
*
* COUNTRY
*
*
*/
class Country : public MapComponent {
public:
	Country(string territoryName);
	Country(const Country& source);

	~Country();

	Country& operator=(const Country& rhs);

	friend ostream& operator<<(ostream& out, const Country& toOut);

	void setParent(Continent* parent);
	void setPlayerOwnership(Player* player);
	string getContinentParentName();
	int getPlayerOwnership();
	void setArmiesOnTerritory(int a);
	int getArmiesOnTerritory();
	int getParentNumCountries();
	int getParentId();
	int getParentBonus();
	void deployArmies(int numDeploying);
	int reduceArmies(int numLeavingToAttack);

	int getCountryId() { return this->territoryId; }
	void sustainOpponentLosses(int numLost);
	void setArmiesAdvancingDuringRound(int advancing) { this->armiesAdvancingDuringRound = advancing; }
	void resetAdvancing() { this->armiesAdvancingDuringRound = 0; }
	int getAdvancing() { return this->armiesAdvancingDuringRound; }

	void bomb();
	void blockade();
	void initiateDiplomacy(int diplomacyId);
	bool diplomaticallyBlocked(int playerId);

private:
	Continent* parent;
	Player* owner;
	int playerId;
	int armies;
	int armiesAdvancingDuringRound;
};



/* ========================================================================================================= */
/*
*
*
* MAP
*
*
*/
class Map {
public:
	Map(string mapName);
	Map(string mapName, GameStatsObserver* observer);
	Map(const Map& source);


	~Map();

	Map& operator=(const Map& rhs);

	friend ostream& operator<<(ostream& out, const Map& toOut);

	bool territoryExists(string territoryName);
	void addCountryByName(string continentName, string territoryName);
	void addCountryByReference(Continent* continent, Country* country);
	void addEdgeByName(string territoryNameOne, string territoryNameTwo);
	void addEdgeByReference(MapComponent* territoryOne, MapComponent* territoryTwo);
	void addContinentByName(string continentName);
	void addContinentByReference(Continent* continent);
	bool edgeExists(MapComponent* territoryOne, MapComponent* territoryTwo);
	bool validate();
	Country* setPlayerOwnership(Player* player, string territoryName);
	vector<Country*> getCountries();
	int getNumContinents();
	int getNumCountries();

	void attachGameStatsObserver(GameStatsObserver* gso);

	int deservesContinentBonus(int playerId);
	void setContinentBonus(string continentName, int bonus);

private:
	string mapName;
	vector<Continent*> vertices;
	vector<MapEdge*> edges;
	vector<MapComponent*> mapTerritories;
	int findTerritory(string territoryName);

	GameStatsObserver* registeredStatsObserver;
};
