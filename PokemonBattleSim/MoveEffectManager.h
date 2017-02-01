#pragma once

#include "Util.h"
#include "ActionManager.h"

class MoveEffectManager
{
public:
	MoveEffectManager();
	~MoveEffectManager();

	static int activateCritEffect(Move move);
	static int activateMultiHitEffect(Move move);
	static int activateOHKOAcc(Move move, int *hitChance, ActivePokemon *user, ActivePokemon *victim);
	static void activate2TurnMove(Move move, ActivePokemon *user, Util::User plr);
	static void activateOnMiss(Move move, ActivePokemon *user);
	static void activateRecoil(Move move, ActivePokemon *user, int damage);
	static void activateForceSwitch(Move move, Team *userTeam, Team *otherTeam);
	static void activateFlatDamage(Move move, ActivePokemon *user, int* damage);
	static void activateCounter(Move move, ActivePokemon *user, int* damage);
	static void activateDamageEffect(Move move, ActivePokemon *user, ActivePokemon *victim);
};

