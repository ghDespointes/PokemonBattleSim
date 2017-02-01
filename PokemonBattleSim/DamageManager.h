#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Util.h"
#include "Move.h"
#include "ActivePokemon.h"
#include "MoveEffectManager.h"

using namespace std;

class DamageManager
{
	static const vector<vector<double>> typeEffectiveness;

public:
	static double getEffectiveness(Util::PokeType type1, Util::PokeType type2);
	static double getMoveEffectiveness(Util::PokeType moveType, Pokemon victim);
	static double getModifier(Move move, Pokemon user, Pokemon victim, bool crit);
	static double calcBaseDamage(Move move, ActivePokemon attacker, ActivePokemon defender, bool crit);
	static int calcConfusion(ActivePokemon user);
	static int calcDamage(ActiveMove move, ActivePokemon user, ActivePokemon victim);

	DamageManager();
	~DamageManager();
};

