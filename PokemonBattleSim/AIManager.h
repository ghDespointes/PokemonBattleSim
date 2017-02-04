#pragma once

#include "Util.h"
#include "Move.h"
#include "Pokemon.h"
#include "ActiveMove.h"
#include "ActivePokemon.h"
#include "Team.h"
#include "TurnAction.h"
#include "DamageManager.h"

class AIManager
{
	Team* team;
	Team* otherTeam;


public:
	AIManager(Team* inputTeam, Team* otherTeam);
	~AIManager();

	double calculateMoveEffectiveness(ActiveMove move);
	double calculateStatusEffectivness(ActivePokemon* active, ActiveMove move);
	double calculateSwitchEffectiveness(ActivePokemon mon);
	int calculateBestMove();
	int calculateBestSwitch();

	TurnAction determineAction(Util::Action act);
};

