#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Util.h"
#include "Move.h"

using namespace std;

class ActiveMove {
	string name;
	string internalName;
	int ID;

	int maxPP;
	int currPP;

public:
	ActiveMove();
	ActiveMove(string name);
	~ActiveMove();

	int useMove();

	string getName();
	string getIntName();
	int getID();

	int getMaxPP();

	void setPP(int pp);
	int getPP();

	void print();
	void printBattleInfo();
};

