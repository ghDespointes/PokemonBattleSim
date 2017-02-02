#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "ActivePokemon.h"

//Holds all active pokemon
class Team {
	//The team itself
	vector<ActivePokemon> team;

public:
	Team();
	Team(vector<ActivePokemon>);
	~Team();

	//Makes pokemon at position the new active pokeon
	void makeActive(int poke);

	//Return the active pokemon to be edited
	ActivePokemon* getActive();

	//Returns copy of the team
	vector<ActivePokemon> getTeam();

	//Return loc of pokemon if exists
	int findPoke(string name);

	//Check if entire team is fainted
	bool checkIfTeamAlive();

	//Different types of print
	void print();
	void printBattleInfo();
	void printEnemyInfo();
};

