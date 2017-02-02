#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Move.h"

using namespace std;

//ActiveMoves are what active pokemon actually contain
//Whenever more information is needed a look up to the corresponding Move
//is looked at
class ActiveMove {
	//Name for player, internalName to access the move data, and ID that has no use yet
	string name;
	string internalName;
	int ID;

	//The max ammount of uses the move has
	int maxPP;
	//How many uses the move has left
	int currPP;

public:
	//Constructor/Destructor
	ActiveMove();
	ActiveMove(string name);
	~ActiveMove();

	//Getters for name, intName, ID, and maxPP because read only
	string getName();
	string getIntName();
	int getID();

	//Getter and setter for current PP
	int getMaxPP();

	//CHANGE TO DECREMENT PP
	void setPP(int pp);
	int getPP();

	//Prints for debugging and for the player
	void print();
	void printBattleInfo();
};

