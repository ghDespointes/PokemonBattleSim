#pragma once
#include <iostream>
#include <string>
#include <queue>

#include "Util.h"
#include "Pokemon.h"
#include "Team.h"
#include "TurnAction.h"
#include "DamageManager.h"
#include "FileReader.h"
#include "MoveEffectManager.h"

class ActionManager
{
	Team *plrTeam;
	Team *enemyTeam;

	vector<Util::User> turnOrder;
	queue<TurnAction> actionQueue;

	bool speedTie = false;

public:
	ActionManager();
	ActionManager(Team *plr, Team *enemy);
	~ActionManager();

	void updateTurnOrder();
	void changeSpeedTie();

	bool checkInputLegality(vector<string> input, TurnAction *action);
	bool checkActionLegality(TurnAction* action);
	void createActionQueue(TurnAction plrAction, TurnAction enmAction);

	TurnAction getPlayerInput();
	TurnAction getPlayerInput(Util::Action action);

	void performSwitch(Util::User user, Team *team, int param);
	void performAttack(Util::User user, Team *team, Team *otherTeam, int param);

	void performAction(TurnAction action);
	void performActions();

	void endTurn();
};

