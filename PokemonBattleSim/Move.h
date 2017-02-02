#pragma once

#include <string>
#include <vector>

#include "Util.h"

using namespace std;

class Move{
private:
	//ID currently has no use
	//Name is the name that players will see and use
	//Internal Name is the main way for determining what a move is
		//it is always the standard name in all capital letters
		//and without spaces
	int ID;
	string name;
	string internalName;

	//An integer code that'll determine any battle effects the move will have
	int function;

	//The base power of the move
	int BP;

	//The pokemon type of the move (ie. normal, flying, fighting ...)
	Util::PokeType type;

	//The type of damage the move does if any (Physical, Special, Status)
	Util::MoveType damType;

	//The accuracy
	int acc;
	//The number of times the moved can be used
	int PP;

	//The likelihood of the move effect occuring
	int effectChance;

	//The priority of the move, [-6,6]
	int priority;

	//Battle description to be printed if the player asks for it
	string description;

	//Possible later implementations
	//Target during multi battle
	//Flags for special effects

public:
	Move();
	Move(vector<string>);
	~Move();

	//This class is read only
	//Standard getters
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
	void printBattleInfo();
};

