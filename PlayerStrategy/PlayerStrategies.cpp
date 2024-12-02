#pragma once

#include "../Player/player.h"
#include "PlayerStrategies.h"
#include "../Order/Orders.h"
#include "../Card/Cards.h"
#include "../Map/Map.h"

#include <time.h>;
#include <algorithm>


using namespace std;

class Player;

/* ========================================================================================================= */
/*
*
* PlayerStrategy
*
*/
PlayerStrategy::PlayerStrategy(Player* p, string strategyName) {
	this->player = p;
    this->strategyName = strategyName;
}

PlayerStrategy::PlayerStrategy(Player* p) : PlayerStrategy(p, "DEFAULT") {}

/**

DEFAULT STRATEGY FROM A2

THINK OF IT AS RANDOM STRATEGY

*/
void PlayerStrategy::issueOrder() {
    // issue deploy orders for available reinforcements
    this->player->deployReinforcements();

    // issue advance orders
    vector<Country*> attackingTerrs = this->toAttack();
    vector<bool> wasDepleted;
    for (int i = 0; i < this->player->ownedTerritories.size(); i++) {
        wasDepleted.push_back(false);
    }

    srand(time(0));

    // attack a random number of territories, must be smaller than the number of attack territories and the number of defenders territories
    int numToAttack = (rand() % attackingTerrs.size());
    int i = 0;
    int failCounter = 0;
    int initialNumDeployments = this->player->deployments.size();
    while (i < initialNumDeployments && i < attackingTerrs.size()) {
        // for each attack
        // get a random territory to withdraw troops from, must have troops deployed on it
        int defendIdx = rand() % this->player->ownedTerritories.size();
        if (!wasDepleted[defendIdx] && this->player->deployments.find(this->player->ownedTerritories[defendIdx]->getCountryId()) != this->player->deployments.end()) {
            wasDepleted[defendIdx] = true;
            // get number to use for attack, allow = all deployed on territory
            int useForAttack = rand() % (this->player->deployments[this->player->ownedTerritories[defendIdx]->getCountryId()] + 1);
            // can't advance 0....                try to deploy 3 at least               less than three, deploy that number     good number to use
            useForAttack = (useForAttack == 0) ? ((this->player->deployments[this->player->ownedTerritories[defendIdx]->getCountryId()] > 3) ? 3 : this->player->deployments[this->player->ownedTerritories[defendIdx]->getCountryId()]) : useForAttack;
            Advance* advanceOrder = new Advance(this->player, this->player->ownedTerritories[defendIdx], attackingTerrs[i], useForAttack);
            this->player->ownedTerritories[defendIdx]->setArmiesAdvancingDuringRound(useForAttack);

            // right now to attack is returning all adjacent territories, including the ones owned by the same player
            // if the target of the advance is an owned territory, don't let it become the source of an advance
            // if it was allowed to become source of advance later, then this would create extra armies
            if (attackingTerrs[i]->getPlayerOwnership() == this->player->playerId) {
                for (int j = 0; j < this->player->ownedTerritories.size(); j++) {
                    if (this->player->ownedTerritories[j]->getTerritoryName().compare(attackingTerrs[i]->getTerritoryName()) == 0) {
                        wasDepleted[j] = true;
                        break;
                    }
                }
            }
            this->player->sendToBattle(this->player->ownedTerritories[defendIdx]->getCountryId(), useForAttack);
            this->player->ol->addOrder(advanceOrder);
            i++;
        }
        else if (failCounter > 50) {
            break;
        }
        else {
            failCounter++;
        }
    }

    if (this->player->hand->hasCards()) {
        // get everything set up to use card
        Country* source = nullptr;
        Country* attacking = nullptr;
        int numArmies = 0;

        // get a country for source
        for (Country* c : this->player->ownedTerritories) {
            if (this->player->deployments.find(c->getCountryId()) != this->player->deployments.end()) {
                if (this->player->deployments[c->getCountryId()] >= 3) {
                    numArmies = (this->player->deployments[c->getCountryId()] == 3) ? 3 : (int)this->player->deployments[c->getCountryId()] / 2;
                    source = c;
                    break;
                }
            }
        }

        // get country to attack
        for (Country* c : attackingTerrs) {
            if (c->getPlayerOwnership() != this->player->playerId && c->getPlayerOwnership() != -1) {
                attacking = c;
                break;
            }
        }

        if (attacking != nullptr && source != nullptr) {
            // set info so card can create order
            Card::setCurrentInfo(this->player, attacking, source, numArmies);

            // draw first card from hand
            Card* card = this->player->hand->getCard();
            Player::gameDeck->returnToDeck(card);
            // create order from card
            this->player->ol->addOrder(card->getOrder());

            // unset info
            Card::unsetCurrentInfo();
        }
    }
}


vector<Country*> PlayerStrategy::toAttack() {
    // build a list of territories
    vector<Country*> toAttack = vector<Country*>();

    // loop through each owned territory
    for (int i = 0; i < this->player->ownedTerritories.size(); i++) {
        // get the adjacent territories to the current owned territory
        vector<Country*> adjacentTerrs = (dynamic_cast<MapComponent*>(this->player->ownedTerritories.at(i)))->getAdjacentCountries();
        // loop through each adjacent territories
        for (int j = 0; j < adjacentTerrs.size(); j++) {
            bool found = false;
            // for each territory that has been added to toAttack
            for (int k = 0; k < toAttack.size(); k++) {
                // make sure the current adjacentTerritory is not already in the list
                if (toAttack.at(k)->getTerritoryName().compare(adjacentTerrs.at(j)->getTerritoryName()) == 0) {
                    found = true;
                    break;
                }
            }
            for (int k = 0; k < this->player->ownedTerritories.size(); k++) {
                if (adjacentTerrs.at(j)->getTerritoryName().compare(this->player->ownedTerritories.at(k)->getTerritoryName()) == 0) {
                    found = true;
                    break;
                }
            }
            // if the territoryname wasn't found
            if (!found) {
                toAttack.push_back(adjacentTerrs.at(j));
            }
        }
    }
    // return a list of unique adjacent territories to the players territory
    return toAttack;
}


vector<Country*> PlayerStrategy::toDefend() {
    return this->player->ownedTerritories;
}


void PlayerStrategy::basicDeployReinforcements() {
    this->player->deployReinforcements();
}

bool PlayerStrategy::lowToHighCompare(const pair<int, int>& first, const pair<int, int>& second) {
    return first.second < second.second;
}


bool PlayerStrategy::highToLowCompare(const pair<int, int>& first, const pair<int, int>& second) {
    return first.second > second.second;
}

vector<pair<int, int>> PlayerStrategy::sort(bool lowHigh) {
    vector<pair<int, int>> mapAsVector;
    for (auto& it : this->player->deployments) {
        mapAsVector.push_back(it);
    }

    if (lowHigh) {
        std::sort(mapAsVector.begin(), mapAsVector.end(), PlayerStrategy::lowToHighCompare);
    }
    else {
        std::sort(mapAsVector.begin(), mapAsVector.end(), PlayerStrategy::highToLowCompare);
    }

    return mapAsVector;
}


/* ========================================================================================================= */
/*
*
* AggressivePlayerStrategy
*
*/
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* p) : PlayerStrategy(p, "AGGRESSIVE") {}


void AggressivePlayerStrategy::issueOrder() {
    // add reinforcements
    // send them to battle
    // draw a card
    int countryIdx = 0;
    int idx = 0;
    bool found = false;
    vector<Country*> toConsolidate;
    for (Country* c : this->player->ownedTerritories) {
        if (c->getArmiesOnTerritory() > 0 && !found) {
            countryIdx = idx;
            found = true;
        }
        else if (c->getArmiesOnTerritory() > 0) {
            toConsolidate.push_back(c);
        }
        idx++;
    }

    Country* baseCountry = this->player->ownedTerritories[countryIdx];
    
    if (this->player->reinforcementPool > 0) {
        // send all reinforcements to one country
        Order* deployOrder = new Deploy(this->player, baseCountry, this->player->reinforcementPool);
        this->player->addToDeployments(baseCountry->getCountryId(), this->player->reinforcementPool);
        this->player->reinforcementPool = 0;
        this->player->ol->addOrder(deployOrder);
    }

    for (Country* c : toConsolidate) {
        // move all armies to the base country
        Order* advanceToConsolidate = new Advance(this->player, c, baseCountry, c->getArmiesOnTerritory());
        this->player->addToDeployments(baseCountry->getCountryId(), c->getArmiesOnTerritory());
        this->player->sendToBattle(c->getCountryId(), c->getArmiesOnTerritory());
        this->player->ol->addOrder(advanceToConsolidate);
    }

    // choose a country to invade
    Country* attacking = nullptr;
    for (Country* c : this->toAttack()) {
        if (c->getPlayerOwnership() != this->player->playerId) {
            attacking = c;
            break;
        }
    }
    // send all armies to attack a single country
    Order* finalAttack = new Advance(this->player, baseCountry, attacking, this->player->deployments[baseCountry->getCountryId()]);
    this->player->ol->addOrder(finalAttack);

    if (this->player->hand->hasCards()) {
        // player can only get one card at a time since only making one attack per round
        Card* card = this->player->hand->getCard();
        // ignore card if airlift, since all armies already involved in the finalAttack order issued above
        // ignore card if blockade, since all held territories contribute to reinforcements 
        if (!(card->getType().compare("AirliftCard") == 0 || card->getType().compare("BlockadeCard") == 0)) {
            Country* enemyTarget = nullptr;
            bool abortNullptr = false;
            if (card->getType().compare("Diplomacy") == 0) {
                // look for a country owned by an enemy, that is not a neutral territory
                // that is also not owned by the enemy that the finalAttack order is targetting
                abortNullptr = true;
                for (Country* c : this->toAttack()) {
                    if (c->getPlayerOwnership() != this->player->playerId && c->getPlayerOwnership() != attacking->getPlayerOwnership() && c->getPlayerOwnership() != -1) {
                        enemyTarget = c;
                        break;
                    }
                }
            }
            else if (card->getType().compare("BombCard") == 0) {
                abortNullptr = true;
                // look for a country owned by an enemy, that is not a neutral territory
                // that is also not owned by the enemy that the finalAttack order is targetting
                for (Country* c : this->toAttack()) {
                    if (c->getPlayerOwnership() != this->player->playerId && c->getCountryId() != attacking->getCountryId()) {
                        enemyTarget = c;
                        break;
                    }
                }
            }
            // use the card if a suitable territory was found or if it doesn't matter if the territory is null (Reinforcement)
            if (enemyTarget != nullptr || !abortNullptr) {
                Card::setCurrentInfo(this->player, enemyTarget, baseCountry, 0);
                this->player->ol->addOrder(card->getOrder());
                Card::unsetCurrentInfo();
            }
          
        }
        Player::gameDeck->returnToDeck(card);
    }
}


/* ========================================================================================================= */
/*
*
* NeutralPlayerStrategy
*
*/
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* p): PlayerStrategy(p, "NEUTRAL") {}


void NeutralPlayerStrategy::issueOrder() {
    // neutral player does nothing......
    cout << "\n   Player " + to_string(this->player->getPlayerId()) + " says: I am neutral, I refuse to issue orders!\n" << endl;
}


/* ========================================================================================================= */
/*
*
* BenevolentPlayerStrategy
*
*/

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* p) : PlayerStrategy(p, "BENEVOLENT") {
    this->isFirst = true;
}



void BenevolentPlayerStrategy::issueOrder() {
    if (this->isFirst) {
        this->basicDeployReinforcements();
        this->isFirst = false;
    }
    else {
        // reinforce any countries still with 0
        for (Country* c : this->player->ownedTerritories) {
            if (this->player->reinforcementPool == 0) {
                break;
            }
            if (c->getArmiesOnTerritory() == 0) {
                // reinforce this country since it has 0
                // try to deploy 10
                int deployable = (this->player->reinforcementPool > 10) ? 10 : this->player->reinforcementPool;
                this->player->reinforcementPool -= deployable;
                Order* deployOrder = new Deploy(this->player, c, deployable);
                this->player->ol->addOrder(deployOrder);
                this->player->addToDeployments(c->getCountryId(), deployable);
            }
        }
        vector<int> uniqueDeploymentLevels;
        // sort deployments from least to most
        vector<pair<int, int>> mapAsVector = this->sort();

        for (auto itr : mapAsVector) {
            bool found = false;
            for (int i = 0; i < uniqueDeploymentLevels.size(); i++) {
                if (uniqueDeploymentLevels[i] == itr.second) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                uniqueDeploymentLevels.push_back(itr.second);
            }
        }

        auto itr = mapAsVector.begin();
        // reinforce the least deployed
        while (this->player->reinforcementPool > 0) {
            if (itr == mapAsVector.end()) {
                itr = mapAsVector.begin();
            }
            // deploy 10 at a time
            int deployable = (this->player->reinforcementPool > 10) ? 10 : this->player->reinforcementPool;
            this->player->reinforcementPool -= deployable;
            Country* deployTo = nullptr;

            for (Country* c : this->player->ownedTerritories) {
                if (c->getCountryId() == itr->first) {
                    deployTo = c;
                    break;
                }
            }

            if (deployTo == nullptr) {
                break;
            }

            Order* deployOrder = new Deploy(this->player, deployTo, deployable);
            this->player->ol->addOrder(deployOrder);
            this->player->addToDeployments(itr->first, deployable);
            itr++;
        }

        // if some countries end up with too many, redistribute
        if (this->player->ownedTerritories.size() > 1) {
            // sort from high to low
            mapAsVector = this->sort(false);

            itr = mapAsVector.begin();
            int most = itr->second;
            int mostId = itr->first;
            itr++;
            int secondMost = itr->second;
            // if num surpasses arbitrary threshold, redistribute
            int threshold = 40;
            if (most - secondMost > threshold) {
                // resort the map again to add to lowest countries first
                mapAsVector = this->sort();

                Country* mostCountry = nullptr;
                for (Country* c : this->player->ownedTerritories) {
                    if (c->getCountryId() == mostId) {
                        mostCountry = c;
                        break;
                    }
                }
                // redsiitribute by 10
                int numRedistributions = threshold / 10;
                numRedistributions += (numRedistributions % 10 > 0) ? 1 : 0;

                int numToRedistribute = most - secondMost;
                // 
                itr = mapAsVector.begin();
                while (numToRedistribute > 0) {
                    if (itr == mapAsVector.end()) {
                        itr = mapAsVector.begin();
                    }
                    // deploy 10 at a time
                    int deployable = (numToRedistribute > 10) ? 10 : numToRedistribute;
                    numToRedistribute = -deployable;
                    Country* deployTo = nullptr;

                    for (Country* c : this->player->ownedTerritories) {
                        if (c->getCountryId() == itr->first) {
                            deployTo = c;
                            break;
                        }
                    }

                    if (deployTo == nullptr) {
                        break;
                    }
                    // create advance order to send troops
                    Order* deployOrder = new Advance(this->player, mostCountry, deployTo, deployable);
                    this->player->ol->addOrder(deployOrder);
                    this->player->addToDeployments(itr->first, deployable);
                    itr++;
                }
            }
        }
    }
}



/* ========================================================================================================= */
/*
*
* HumanPlayerStrategy
*
*/

HumanPlayerStrategy::HumanPlayerStrategy(Player* p) : PlayerStrategy(p, "HUMAN") {}


void HumanPlayerStrategy::issueOrder() {
    // while
    //    show deployments
    //    show threats
    //    force deployments of all reinforcements
    // while
    //    show deployments
    //    show threats
    //    allow as many advances orders
    // show cards
    // pick a card
    // pick a country
    cout << "\nHUMAN, PLEASE SELECT YOUR MOVES FOR THIS ROUND\n" << endl;
    this->deployReinforcements();
    this->chooseAdvances();
    this->useCard();
    cout << "\n\n";
}


void HumanPlayerStrategy::useCard() {
    if (player->hand->hasCards()) {
        cout << "\n\nSELECT A CARD:" << endl;
        cout << "\n  Cards in hand:" << endl;
        vector<Card*> cards = this->player->hand->getCards();
        for (int i = 0; i < cards.size(); i++) {
            cout << "    Card " + to_string((i + 1)) + ": " + cards[i]->getType();
        }

        int selection;
        bool isValid = false;
        while (!isValid) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Do you want to use a card? Enter 1 to continue, 0 to quit: ";
            cin >> selection;
            isValid = cin.good() && (selection == 1 || selection == 0);
            if (selection == 0) {
                return;
            }
        }

        isValid = false;
        while (!isValid) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Do you want to use a card? Enter 1 to continue, 0 to quit: ";
            cin >> selection;
            isValid = cin.good() && (selection > 0 && selection <= cards.size());
        }

        map<int, Country*> cMap;
        map<int, Country*> attackMap;
        for (Country* c : this->player->ownedTerritories) {
            int numArmies = 0;
            if (this->player->deployments.find(c->getCountryId()) != this->player->deployments.end()) {
                numArmies = this->player->deployments[c->getCountryId()];
            }
            cout << "    Armies stationed on " + c->getTerritoryName() + "(" + to_string(c->getCountryId()) + ")::  " + to_string(numArmies) << endl;
            cMap.insert(pair<int, Country*>(c->getCountryId(), c));
        }

        cout << "\n\n  Available territories to attack:" << endl;

        for (Country* c : this->toAttack()) {
            if (c->getPlayerOwnership() != this->player->playerId) {
                cout << "    Armies stationed on " + c->getTerritoryName() + "(" + to_string(c->getCountryId()) + ")::  " + to_string(c->getArmiesOnTerritory()) << endl;
                attackMap.insert(pair<int, Country*>(c->getCountryId(), c));
            }
        }
        cout << "\n\n";
        Country* source = nullptr;
        Country* dest = nullptr;
        int num = 0;

        if (cards[selection]->getType().compare("Reinforcement") == 0 || cards[selection]->getType().compare("BlockadeCard") == 0) {
            isValid = false;
            int countrySelection;
            while (!isValid) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Select one of your territories to be the subject of the " + cards[selection]->getType() + ": ";
                cin >> countrySelection;
                if (cin.good()) {
                    if (cMap.find(countrySelection) != cMap.end()) {
                        isValid = true;
                        break;
                    }
                }
                source = cMap[countrySelection];
            }
        }
        else if (cards[selection]->getType().compare("BombCard") == 0 || cards[selection]->getType().compare("Diplomacy") == 0) {
            isValid = false;
            int countrySelection;
            while (!isValid) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Select one of your enemies' territories to be the subject of the " + cards[selection]->getType() + ": ";
                cin >> countrySelection;
                if (cin.good()) {
                    if (cMap.find(countrySelection) != attackMap.end()) {
                        isValid = true;
                        source = attackMap[countrySelection];
                        break;
                    }
                }
            }
        }
        else {
            // airlift
            isValid = false;
            int countrySelection;
            while (!isValid) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Select one of your territories by its ID to airlift armies from: ";
                cin >> countrySelection;
                if (cin.good()) {
                    if (cMap.find(countrySelection) != cMap.end()) {
                        isValid = true;
                        source = cMap[countrySelection];
                        break;
                    }
                }
            }

            isValid = false;
            int numAttackingArmies;
            while (!isValid) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Select the number of reinforcements to deploy: ";
                cin >> numAttackingArmies;
                isValid = cin.good() && numAttackingArmies > 0 && numAttackingArmies <= this->player->deployments[countrySelection];
            }
            num = numAttackingArmies;

            int destination;
            isValid = false;
            while (!isValid) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Select one of your territories, or an enemy territory, by its ID to airlift armies to: ";
                cin >> destination;
                if (cin.good()) {
                    if (cMap.find(destination) != cMap.end() || attackMap.find(destination) != attackMap.end()) {
                        isValid = true;
                        dest = (cMap.find(destination) != cMap.end()) ? cMap[destination] : attackMap[destination];
                        break;
                    }
                }
            }
        }

        // set info so card can create order
        Card::setCurrentInfo(this->player, dest, source, num);

        // draw first card from hand
        Card* card = this->player->hand->removeFromHand(selection);
        Player::gameDeck->returnToDeck(card);
        // create order from card
        this->player->ol->addOrder(card->getOrder());

        // unset info
        Card::unsetCurrentInfo();
    }
}


void HumanPlayerStrategy::chooseAdvances() {
    bool keepChoosing = true;
    while (keepChoosing) {

        cout << "\nSELECT AN ADVANCE:  \n";

        int selection;
        bool isValid = false;
        while (!isValid) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Do you want to continue choosing advances? Enter 1 to continue, 0 to quit: ";
            cin >> selection;
            isValid = cin.good() && (selection == 1 || selection == 0);
            if (selection == 0) {
                keepChoosing = false;
                break;
            }
        }
        if (!keepChoosing) {
            break;
        }

        cout << "\n  Current player positioning:" << endl;
        map<int, Country*> cMap;
        map<int, Country*> attackMap;
        for (Country* c : this->player->ownedTerritories) {
            int numArmies = 0;
            if (this->player->deployments.find(c->getCountryId()) != this->player->deployments.end()) {
                numArmies = this->player->deployments[c->getCountryId()];
            }
            cout << "    Armies stationed on " + c->getTerritoryName() + "(" + to_string(c->getCountryId()) + ")::  " + to_string(numArmies) << endl;
            cMap.insert(pair<int, Country*>(c->getCountryId(), c));
        }

        cout << "\n\n  Available territories to attack:" << endl;

        for (Country* c : this->toAttack()) {
            if (c->getPlayerOwnership() != this->player->playerId) {
                cout << "    Armies stationed on " + c->getTerritoryName() + "(" + to_string(c->getCountryId()) + ")::  " + to_string(c->getArmiesOnTerritory()) << endl;
                attackMap.insert(pair<int, Country*>(c->getCountryId(), c));
            }
        }
        cout << "\n\n";

        isValid = false;
        int countrySelection;
        while (!isValid) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Select one of your territories by its ID to advance armies from: ";
            cin >> countrySelection;
            if (cin.good()) {
                if (cMap.find(countrySelection) != cMap.end()) {
                    isValid = true;
                    break;
                }
            }
        }

        isValid = false;
        int numAttackingArmies;
        while (!isValid) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Select the number of reinforcements to deploy: ";
            cin >> numAttackingArmies;
            isValid = cin.good() && numAttackingArmies > 0 && numAttackingArmies <= this->player->deployments[countrySelection];
        }
         
        int destination;
        isValid = false;
        while (!isValid) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Select one of your territories, or an enemy territory, by its ID to advance armies to: ";
            cin >> destination;
            if (cin.good()) {
                if (cMap.find(destination) != cMap.end() || attackMap.find(destination) != attackMap.end()) {
                    isValid = true;
                    break;
                }
            }
        }

        Country* destinationC = (cMap.find(destination) != cMap.end()) ? cMap[destination] : attackMap[destination];

        Order* advanceOrder = new Advance(this->player, cMap[countrySelection], destinationC, numAttackingArmies);
        this->player->ol->addOrder(advanceOrder);
        this->player->sendToBattle(countrySelection, numAttackingArmies);
    }
}

void HumanPlayerStrategy::deployReinforcements() {

    while (this->player->reinforcementPool > 0) {
        cout << "\nREINFORCEMENTS TO DEPLOY:  " + to_string(this->player->reinforcementPool) + "\n";
        cout << "\n  Current player positioning:" << endl;
        map<int, Country*> cMap;
        for (Country* c : this->player->ownedTerritories) {
            int numArmies = 0;
            if (this->player->deployments.find(c->getCountryId()) != this->player->deployments.end()) {
                numArmies = this->player->deployments[c->getCountryId()];
            }
            cout << "    Armies stationed on " + c->getTerritoryName() + "(" + to_string(c->getCountryId()) + ")::  " + to_string(numArmies) << endl;
            cMap.insert(pair<int, Country*>(c->getCountryId(), c));
        }

        cout << "\n\n  Current neighboring opponent positioning:" << endl;

        for (Country* c : this->toAttack()) {
            if (c->getPlayerOwnership() != this->player->playerId) {
                cout << "    Armies stationed on " + c->getTerritoryName() + "(" + to_string(c->getCountryId()) + ")::  " + to_string(c->getArmiesOnTerritory()) << endl;
            }
        }
        cout << "\n\n";

        bool isValid = false;
        int countrySelection;
        while (!isValid) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Select one of your territories by its ID to deploy armies to: ";
            cin >> countrySelection;
            if (cin.good()) {
                if (cMap.find(countrySelection) != cMap.end()) {
                    isValid = true;
                    break;
                }
            }
        }

        isValid = false;
        int reinforcementNumber;
        while (!isValid) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Select the number of reinforcements to deploy: ";
            cin >> reinforcementNumber;
            isValid = cin.good() && reinforcementNumber > 0 && reinforcementNumber <= this->player->reinforcementPool;
        }
        Order* deployOrder = new Deploy(this->player, cMap[countrySelection], reinforcementNumber);
        this->player->ol->addOrder(deployOrder);
        this->player->addToDeployments(countrySelection, reinforcementNumber);
        this->player->reinforcementPool -= reinforcementNumber;
    }

}