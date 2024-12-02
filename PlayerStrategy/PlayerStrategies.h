#pragma once

#include "../Player/player.h"

class Player;

class PlayerStrategy {
public:
	PlayerStrategy(Player* p);
	PlayerStrategy(Player* p, string strategyName);

	virtual void issueOrder();
	vector<Country*> toAttack();
	vector<Country*> toDefend();

	static bool lowToHighCompare(const pair<int, int>& first, const pair<int, int>& second);
	static bool highToLowCompare(const pair<int, int>& first, const pair<int, int>& second);

protected:
	Player* player;
	string strategyName;

	void basicDeployReinforcements();
	vector<pair<int, int>> sort(bool lowHigh = true);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
	AggressivePlayerStrategy(Player* p);

	void issueOrder();
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
	NeutralPlayerStrategy(Player* p);

	void issueOrder();
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
	BenevolentPlayerStrategy(Player* p);

	void issueOrder();

private:
	bool isFirst;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
	HumanPlayerStrategy(Player* p);

	void issueOrder();

private:
	void deployReinforcements();
	void chooseAdvances();
	void useCard();
};
