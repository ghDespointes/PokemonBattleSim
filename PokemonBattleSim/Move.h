#pragma once

#include <string>
#include <vector>

#include "Util.h"

using namespace std;

class Move{
private:
	int ID;
	string name;
	string internalName;

	int function;
	int BP;

	Util::PokeType type;
	Util::MoveType damType;

	int acc;
	int PP;

	int effectChance;
	int priority;

	string description;

	//Possible later implementations
	//Target during multi battle
	//Flags for special effects

public:
	Move();
	Move(vector<string>);
	~Move();

	int getID();
	string getName();
	string getInternalName();

	int getFunction();
	int getBP();

	Util::PokeType getType();
	Util::MoveType getMoveType();

	int getAcc();
	int getPP();

	int getEffChance();
	int getPriority();

	string getDescription();

	void print();
};

