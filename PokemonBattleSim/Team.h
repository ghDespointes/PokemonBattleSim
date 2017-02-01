#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Util.h"
#include "ActivePokemon.h"

class Team {
	vector<ActivePokemon> team;

public:
	Team();
	Team(vector<ActivePokemon>);
	~Team();

	void makeActive(int poke);
	ActivePokemon* getActive();
	vector<ActivePokemon> getTeam();

	int findPoke(string name);

	void print();
	void printBattleInfo();
	void printEnemyInfo();
};

