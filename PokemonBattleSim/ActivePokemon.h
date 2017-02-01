#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include "Util.h"
#include "ActiveMove.h"
#include "Pokemon.h";
#include "TurnAction.h"

using namespace std;

class ActivePokemon{
	string name;
	string internalName;
	int ID;

	int lvl;

	string item;
	string ability;

	string nature;
	
	vector<int> evs;
	vector<int> ivs;

	int health;
	vector<int> stats;
	vector<int> statBoosts;

	vector<ActiveMove> moves;

	Util::StatusCond status;
	vector<int> volStatus;

	int poisonTimer = 0;
	int sleepTimer = 0;

	bool chargingMove = false;
	bool isFly = false;
	bool isDig = false;
	TurnAction storedAction;

	int storedMove = -1;
	int storedDamage = 0;
	int disabledMove = -1;
	int disableTimer = 0;

public:
	ActivePokemon();
	ActivePokemon(vector<string> input);
	~ActivePokemon();

	void fixEvs();
	void fixIVs();

	void calculateStats(Pokemon pokeInfo);

	string getName();
	string getIntName();
	int getID();
	int getLevel();

	string getItem();

	int getStat(Util::StatType stat);
	int getEV(Util::StatType stat);
	int getIV(Util::StatType stat);

	void switchReset();

	double getStatBoost(Util::StatBoost stat);
	void changeStatBoost(Util::StatBoost stat, int quant);
	void resetStatBoost();

	int getHealth();
	void setHealth(int newhealth);
	void damage(int ammount);
	void damagePercent(double percent);
	void heal(int ammount);
	void healPercent(double percent);

	void setStatus(Util::StatusCond newStatus);
	Util::StatusCond getStatus();

	void setVolStatus(Util::VolStatus volStat);
	bool decrementVolStatus(Util::VolStatus volStat);
	int getVolStatus(Util::VolStatus volStat);
	void resetVolStatus();

	void incrementPoison();
	void resetPoison();
	int getPoison();

	bool decrementSleep();
	void resetSleep();
	int getSleep();

	bool getCharge();
	bool getFly();
	bool getDig();
	TurnAction getStoredAction();

	void setCharge(bool input);
	void setFly(bool input);
	void setDig(bool input);
	void setStoredAction(TurnAction action);
	void resetStoredAction();

	void setStoredMove(int move);
	int getStoredMove();

	void setStoredDamage(int dam);
	int getStoredDamage();

	void setDisable(int move);
	int getDisableMove();
	bool decrementDisableTimer();

	int findMove(string name);
	vector<ActiveMove> getMoves();

	void print();
	void printBattleInfo();
	void printBasicInfo();
	void printEnemyInfo();
	void printEnemyBasicInfo();
};

