#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "Util.h"
#include "Move.h"
#include "Pokemon.h"
#include "ActiveMove.h"
#include "ActivePokemon.h"
#include "MoveEffectManager.h"

using namespace std;

//Utility class in charge of calculating any damage
class DamageManager
{
	//Matrix of damage effectiveness
	static const vector<vector<double>> typeEffectiveness;

public:
	//Gets affectiveness of using a move against a type
	static double getEffectiveness(Util::PokeType type1, Util::PokeType type2);
	//Calculates full damage modifier of types
	static double getMoveEffectiveness(Util::PokeType moveType, Pokemon victim);
	//Calculates damage modifier for crit, affectiveness, and random chance
	static double getModifier(Move move, Pokemon user, Pokemon victim, bool crit);
	//Calculate the base damage
	static double calcBaseDamage(Move move, ActivePokemon attacker, ActivePokemon defender, bool crit);
	//Calculate the damage of confusion
	static int calcConfusion(ActivePokemon user);
	//Calclate the total final daamage
	static int calcDamage(ActiveMove move, ActivePokemon user, ActivePokemon victim);
};

