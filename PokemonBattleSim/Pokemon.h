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
	int ID;
	string name;
	string internalName;

	vector<Util::PokeType> types;
	vector<int> stats;

	//Probably Change to something
	//Read in as pairs
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

	string getName();

	string getInternalName();

	int getID();

	int getStat(Util::StatType);

	vector<Util::PokeType> getType();

	vector<string> getMoves();

	void print();
};

