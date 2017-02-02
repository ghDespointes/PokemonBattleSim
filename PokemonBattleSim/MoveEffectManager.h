#pragma once

#include "Util.h"
#include "Move.h"
#include "ActivePokemon.h"
#include "TurnAction.h"
#include "Team.h"

//Utility class in charge of any move effects
class MoveEffectManager
{
public:
	//Sees if critical hit ratios should be higher
	static int activateCritEffect(Move move);
	//Determine if moves should hit multiple times
	static int activateMultiHitEffect(Move move);
	//Determine if moves should be one hit kills and determine their accuracy
	static int activateOHKOAcc(Move move, int *hitChance, ActivePokemon *user, ActivePokemon *victim);
	//Not being used
	//Causes moves to take a charging stage and then actually using it the next turn
	static void activate2TurnMove(Move move, ActivePokemon *user, Util::User plr);
	//Determine if anything should occur when the user misses
	static void activateOnMiss(Move move, ActivePokemon *user);
	//Determines if aything should occur if users deal damage(Specifically here is take recoil or heal)
	static void activateRecoil(Move move, ActivePokemon *user, int damage);
	//Determines if ay pokemon should be switched out
	static void activateForceSwitch(Move move, Team *userTeam, Team *otherTeam);
	//Determines if moves deal a flat damage
	static void activateFlatDamage(Move move, ActivePokemon *user, int* damage);
	//Determines if moves should deal damage based on previous damage they've taken
	static void activateCounter(Move move, ActivePokemon *user, int* damage);
	//Determines if moves should have an additional effect
	static void activateDamageEffect(Move move, ActivePokemon *user, ActivePokemon *victim);
};

