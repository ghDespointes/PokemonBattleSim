#pragma once
#include <iostream>
#include <string>
#include <queue>

#include "Util.h"
#include "Team.h"
#include "TurnAction.h"
#include "ActivePokemon.h"
#include "DamageManager.h"
#include "FileReader.h"
#include "MoveEffectManager.h"
#include "AIManager.h"

class ActionManager
{
	//Each team to access freely
	Team *plrTeam;
	Team *enemyTeam;

	AIManager* aiMan;

	//Vector keeping track of the turn order of the active pokemon
	vector<Util::User> turnOrder;
	//An action queue that will be filled and performed in order
	queue<TurnAction> actionQueue;

	//flag to state whether there is a speed tie or not
	bool speedTie = false;

public:
	ActionManager();
	ActionManager(Team *plr, Team *enemy, AIManager *aiMann);
	~ActionManager();

	//Update turn order to keep it up to date
	void updateTurnOrder();
	//Randomly decides to shuffle turn order if there is a tie
	void changeSpeedTie();

	//Checks legality of player input
	bool checkInputLegality(vector<string> input, TurnAction *action);
	//Checks legality of current actions
	bool checkActionLegality(TurnAction* action);
	//Populates and prepares the ActionQueue
	void createActionQueue(TurnAction plrAction, TurnAction enmAction);

	//Get's player input of all types
	TurnAction getPlayerInput();
	//Get's player input for a specific action
	TurnAction getPlayerInput(Util::Action action);

	//Perform a switch and make anothe pokemon active
	void performSwitch(Util::User user, Team *team, int param);
	//Perform some sort of attack
	void performAttack(Util::User user, Team *team, Team *otherTeam, int param);

	//Prepares and performs an action
	void performAction(TurnAction action);
	//Performs all the actions in the actionQueue
	void performActions();

	//Do any end of turn clean up duties
	void endTurn();

	//Check if any team has lost all their pokemon
	bool checkTeamLoss();
};

