#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Util.h"
#include "ActiveMove.h"
#include "Pokemon.h"
#include "TurnAction.h"

using namespace std;

//Very important class, this acts as the actual pokemon that fight
//We only need to access the corresponding Pokemon once in the constructor
	//So look ups are minimal
class ActivePokemon{
	//Name for players, internmal names for the code, and ID (although not used)
	string name;
	string internalName;
	int ID;

	//Level is used in calculations
	int lvl;

	//items and abilities have no function
		//but were included in case I got to them
	string item;
	string ability;

	//Nature affects the stats of the pokemon
	string nature;
	
	//Evs and Ivs affect the stats of pokemon
	vector<int> evs;
	vector<int> ivs;

	vector<int> stats;

	//This starts the dynamic area everything before is read only
	//health is current health, max health is in stats[0]
	int health;

	//Statboosts refer to modifiers currently affecting the pokemon
	vector<int> statBoosts;

	//A vector of the active moves the activePokemon can have
	//There is a limit of 4 which is enforced in the constructor
	vector<ActiveMove> moves;

	//Status is a permanent effect
		//Pokemon can only have a single status at a time
	Util::StatusCond status;

	//Volatile status are dynamic status conditions that are lost
		//when the pokemon are switched out of combat
	//These stack, so a pokemon can be affected by many
	vector<int> volStatus;

	//Timers and states to keep track of for status conditions and certain moves
	int poisonTimer = 0;
	int sleepTimer = 0;

	//I attempted to implement two turn attacks, but something went wrong
		//so they aren't used currently
	bool chargingMove = false;
	bool isFly = false;
	bool isDig = false;
	TurnAction storedAction;

	//Records the last used moved the pokemon did
	//Records the last ammount of damage the pokemon took.
	int storedMove = -1;
	int storedDamage = 0;
	int disabledMove = -1;
	int disableTimer = 0;

public:
	ActivePokemon();
	ActivePokemon(vector<string> input);
	~ActivePokemon();

	//EVs are capped at 252 per EV and capped at 510 total evs
	void fixEvs();
	//Evs are ranged [0,31]
	void fixIVs();

	//Determines the final stats of the active Pokemon based on
		//Base stats, nature, Evs, and Ivs
	void calculateStats(Pokemon pokeInfo);

	//Getters and setters
	string getName();
	string getIntName();
	int getID();
	int getLevel();

	string getItem();

	int getStat(Util::StatType stat);
	int getEV(Util::StatType stat);
	int getIV(Util::StatType stat);

	//Used to determine if the active pokemon is that type
	bool isType(Util::PokeType type);

	//Resets certain values whenever the pokemon switches out
	void switchReset();

	//Getters, setters, and resetter of stat modifiers
	double getStatBoost(Util::StatBoost stat);

	//Calculate the modified accuracy of a move
	static double calculateAccuracy(int level);
	void changeStatBoost(Util::StatBoost stat, int quant);
	void resetStatBoost();

	//Health related getters and setters
	int getHealth();

	//Returns health as a percantage to various decimal precision values
	double getHealthPercent();
	double getHealthPercentRounded();

	void setHealth(int newhealth);
	void damage(int ammount);
	void damagePercent(double percent);
	void heal(int ammount);
	void healPercent(double percent);

	//Status condition getters and setters
	void setStatus(Util::StatusCond newStatus);
	Util::StatusCond getStatus();

	//Volatile status getters and setters
	void setVolStatus(Util::VolStatus volStat);
	bool decrementVolStatus(Util::VolStatus volStat);
	int getVolStatus(Util::VolStatus volStat);
	void resetVolStatus();

	//Status condition counters 
	void incrementPoison();
	void resetPoison();
	int getPoison();

	bool decrementSleep();
	void resetSleep();
	int getSleep();

	//Can be ignored but potentially used in two turn attacks
	bool getCharge();
	bool getFly();
	bool getDig();
	TurnAction getStoredAction();

	void setCharge(bool input);
	void setFly(bool input);
	void setDig(bool input);
	void setStoredAction(TurnAction action);
	void resetStoredAction();

	//Get and Set last used move
	void setStoredMove(int move);
	int getStoredMove();

	//Get and set last damage taken
	void setStoredDamage(int dam);
	int getStoredDamage();

	//Get, set, and time down for any move that is disabled
	void setDisable(int move);
	int getDisableMove();
	bool decrementDisableTimer();

	//Used to determine if the pokemon has the current move
	int findMove(string name);
	//Return full list of known moves
	vector<ActiveMove> getMoves();

	//A workaround for a weird pointer error i was having
	//Decrements PP of that move
	int getPP(int move);
	//Decrements PP of that move
	void decrementPP(int move);

	//Many print functions that depend on the situation
	void print();
	void printBattleInfo();
	void printBasicInfo();
	void printEnemyInfo();
	void printEnemyBasicInfo();
};

