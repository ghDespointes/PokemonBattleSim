#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>

#include "Util.h"
#include "Move.h"

using namespace std;

class Pokemon {
private:
	//ID currently has no use
	//Name is the name that players will see and use
	//Internal Name is the main way for determining what a pokemon is
		//it is always the standard name in all capital letters
		//and without spaces
	int ID;
	string name;
	string internalName;

	//Vector of types, although will always be 2
		//If a pokemon only has a type, the second will be NONE type
	vector<Util::PokeType> types;

	//Vector of the 6 base stats of that pokemon type
	vector<int> stats;

	//Vector of move internal names that the pokemon can have access too
		//Currently not enforced because there are not enough moves
	vector<string> moves;

	//Ability
	//HiddenAbility

	//Later things to implement
	//Gender rate
	//Growth rate
	//BaseExp
	//Ev give
	//CatchRate

	//Egg Groups
	//Steps to hatch
	//Height
	//Weight
	//Color
	//Kind
	//Pokedex Entry

	//EggMoves
	//Evolutions
	//Forms

public:
	Pokemon();
	Pokemon(vector<string> input);
	~Pokemon();

	//This class is read only
	//Standard getters
	string getName();
	string getInternalName();
	int getID();
	int getStat(Util::StatType);
	vector<Util::PokeType> getType();
	vector<string> getMoves();
	void print();
};

