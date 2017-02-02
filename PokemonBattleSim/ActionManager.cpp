#include "ActionManager.h"



ActionManager::ActionManager()
{
}

//Set up the teams
ActionManager::ActionManager(Team *plr, Team *enemy) {
	plrTeam = plr;
	enemyTeam = enemy;

	//Populate turn order initially
	turnOrder.push_back(Util::Player_1);
	turnOrder.push_back(Util::Enemy_1);

	//Update turn order to make it reflect speed stats
	updateTurnOrder();
}

//Release the pointers
ActionManager::~ActionManager(){
}

//Compares speed stats to determine who should go first when at the same priority level
void ActionManager::updateTurnOrder() {
	//Gets base speed
	int plrSpeed = plrTeam->getActive()->getStat(Util::Speed);
	int enmSpeed = enemyTeam->getActive()->getStat(Util::Speed);

	//Applies any speed modifiers
	plrSpeed *= plrTeam->getActive()->getStatBoost(Util::SpeedBoost);
	enmSpeed *= enemyTeam->getActive()->getStatBoost(Util::SpeedBoost);

	//Check is paralyzed and modify speed accordingly
	if (plrTeam->getActive()->getStatus() == Util::Paralyzed) {
		plrSpeed /= 2;
	}

	if (enemyTeam->getActive()->getStatus() == Util::Paralyzed) {
		enmSpeed /= 2;
	}
	
	//Reset tie flag
	speedTie = false;

	//Looking at speeds, put people in order
	if (plrSpeed > enmSpeed) {
		turnOrder[0] = Util::Player_1;
		turnOrder[1] = Util::Enemy_1;

	} else if(plrSpeed < enmSpeed) {
		turnOrder[1] = Util::Enemy_1;
		turnOrder[0] = Util::Player_1;

	} 
	//If there's a speed tie, set it
	else {
		speedTie = true;
	}
}

//Randomizes turn order if there is a speed tie
void ActionManager::changeSpeedTie() {
	//Get a random number
	//50% chance of not changing aything
	//50% chance of switching the order
	int rand50 = rand() % 100;

	if (rand50 < 50) {
		Util::User temp = turnOrder[0];
		turnOrder[0] = turnOrder[1];
		turnOrder[1] = temp;
	}
}

//Takes player input and an action
//Returns true if it passes
//fills in the action the player is trying to peform
bool ActionManager::checkInputLegality(vector<string> input, TurnAction *action) {
	int num;

	//If you are trying to use a move
	if (input[0] == "USE") {
		//If you try to use a move with no parameter, crash
		if (input.size() == 1) {
			cout << "That's an invalid move, please pick another." << endl;
			return false;
		}

		//If the parameter is not a number put any other pieces of the input together
		if (!isdigit(input[1][0])) {
			for (int i = 2; i < input.size(); i++) {
				input[1] = input[1] + input[i];
			}
		}
		//If it's a number, make sure it corresponds to a move the player has
		else {
			num = stoi(input[1]);
			if (num > plrTeam->getActive()->getMoves().size() || num <= 0) {
				cout << "Please pick a valid attack" << endl;
				return false;
			}
		}

		//Set the action information to be used later
		action->setUser(Util::Player_1);
		action->setAction(Util::Attack);
		action->setParam(input[1]);

	}
	//If you are trying to switch pokemon out
	else if (input[0] == "SWITCH") {
		//If you try to switch with no parameter, crash
		if (input.size() == 1) {
			cout << "That's an invalid pokemon, please pick another." << endl;
			return false;
		}

		//If the parameter is not a number, put back together the pieces of the input
		if (!isdigit(input[1][0])) {
			for (int i = 2; i < input.size(); i++) {
				input[1] = input[1] + input[i];
			}
		}
		//If it's a number make sure that a corresponding pokemon exists
		else {
			num = stoi(input[1]);
			if (num >= plrTeam->getTeam().size() || num == 0) {
				cout << "Please pick a valid pokemon." << endl;
				return false;
			}

			//Prevent players from switching to fainted pokemon
			if (plrTeam->getTeam()[num].getStatus() == Util::Fainted) {
				cout << "That pokemon is fainted, please pick another." << endl;
				return false;
			}
		}
		
		//Fill out the action with the proper information
		action->setUser(Util::Player_1);
		action->setAction(Util::Switch); 
		action->setParam(input[1]);
	}

	return true;
}

//Similar to check input legality
//Takes in an action that can be edited
//Return true if valid or false if not
bool ActionManager::checkActionLegality(TurnAction* action) {
	//Get the actions info
	Util::User user = action->getUser();
	Util::Action actType = action->getAction();
	string param = action->getParam();

	Team *team;

	//Determine if the parameter is a number
	bool isInt = isdigit(param[0]);
	int paramInt;

	//If it is convert it to an int
	if (isInt) {
		paramInt = stoi(param);
	}

	//Determine which team to affect
	if (user == Util::Player_1) {
		team = plrTeam;
	} else {
		team = enemyTeam;
	}

	//If you are trying to attack
	if (actType == Util::Attack) {
		//If the parameter is not a number
		if (!isInt) {
			//Find the move and fail if it doesn't exist
			if (team->getActive()->findMove(param) == -1) {
				cout << "That move doesn't exist, please try again." << endl;

				return false;
			}

			//If the move exists make that the paramInt
			paramInt = team->getActive()->findMove(param);
		}
		else {
			//If its a number subtract 1
			//Player input goes from 1-4, whereas the vector goes from 0-3 so we need to subtract 1
			paramInt -= 1;
		}

		//Check if the move is disabled and fail if it is
		if (team->getActive()->getDisableMove() == paramInt) {
			cout << "That move is disabled, please pick another." << endl;
			return false;
		}

	//If you are trying to switch
	} else {
		//If the parameter is not an int
		if (!isInt) {
			//Fail the check if the player is trying to switch the active pokemon
			if (team->getActive()->getIntName() == param) {
				cout << "That pokemon is already out, please try again." << team->getActive()->getIntName() << endl;

				return false;
			}

			//Fail if the pokemon doesn't exist
			if (team->findPoke(param) == -1) {
				cout << "That pokemon isn't on your team, please try again." << endl;

				return false;
			}

			//Don't let players switch to pokemon that are fainted
			if (team->getTeam()[team->findPoke(param)].getStatus() == Util::Fainted) {
				cout << "That pokemon is fainted, please try again." << endl;

				return false;
			}

			//If the pokemon exists update the paramInt
			paramInt = team->findPoke(param);
		}
	}

	//Change the parameter to its numeric version and pass the action legality
	action->setParam(to_string(paramInt));

	return true;
}

//Takes the players action and the enemies action
//Populates the action queue in the correct order
void ActionManager::createActionQueue(TurnAction plrAction, TurnAction enmAction) {
	vector<TurnAction> turnActions;
	TurnAction action;
	string intName;

	Team *team;

	//Adds the actions to the turns actions
	turnActions.push_back(plrAction);
	turnActions.push_back(enmAction);

	//Check if there's a speed tie and randomize the order if so
	if (speedTie) {
		changeSpeedTie();
	}

	//Pass through one to get any switch which have the highest priority
	for (int i = 0; i < turnOrder.size(); i++) {
		action = turnActions[turnOrder[i]];

		if (action.getAction() == Util::Switch) {
			actionQueue.push(action);
		}
	}

	//Pass through for each other priority level
		//Add moves at that priority level
	for (int j = 6; j >= -6; j--) {
		for (int i = 0; i < turnOrder.size(); i++) {
			action = turnActions[turnOrder[i]];

			if (action.getUser() == Util::Player_1) {
				team = plrTeam;
			}
			else {
				team = enemyTeam;
			}

			intName = team->getActive()->getMoves()[stoi(action.getParam())].getIntName();

			if (action.getAction() == Util::Attack) {
				if (Util::getMove(intName).getPriority() == j) {
					actionQueue.push(action);
				}
			}
		}
	}
}

//Get the players input and convert that into a turn action
//Return the turn action to be used later
TurnAction ActionManager::getPlayerInput() {
	string line;
	vector<string> inputs;
	TurnAction *outputAction = new TurnAction();

	//Set up some possible failure states
	TurnAction crashAction(Util::Player_1, Util::Other, "NONE");
	TurnAction storedMove = plrTeam->getActive()->getStoredAction();

	//This can be ignored
	//Prelimary method of setting up 2 turn moves
	if (storedMove.getParam() != "CHARGE_MOVE") {
		cout << plrTeam->getActive()->getStoredAction().getParam() << endl;
		plrTeam->getActive()->resetStoredAction();

		return storedMove;
	}

	//Get the players input
	cout << "What will you do this turn? " << endl;
	getline(cin, line);

	//Split it by spaces
	inputs = FileReader::split(line, ' ');

	//If its empty return a bad action
	if (inputs.size() == 0) {
		return crashAction;
	}

	//Format each part of the player inputs in all caps and no spaces
	for (int i = 0; i < inputs.size(); i++) {
		inputs[i] = FileReader::formatName(inputs[i]);
	}

	//If the player wants to quit return that action
	if (inputs[0] == "QUIT") {
		return TurnAction(Util::Player_1, Util::Other, "QUIT");
	}
	//If player wants to ask for help print out help statement
	else if (inputs[0] == "HELP") {
		cout << endl;
		cout << "Here's some help:" << endl;
		cout << "Type \"Use X\" to use an attack, where X is the name or number of that move." << endl;
		cout << "Type \"Switch X\" to switch to another pokemon, where X is the name or number of that pokemon." << endl;
		cout << "Type \"Info X\" to see a description of a move, where X is the name or number of that move." << endl;
		cout << endl;

		return crashAction;

	}
	//If player wants more information about certain moves or pokemon
	//Print out that information
	else if (inputs[0] == "INFO") {
		inputs[0] = "USE";

		if (checkInputLegality(inputs, outputAction)) {
			Move move;
			string param = outputAction->getParam();
			int paramInt;

			//Allow the user to check the information of their own moves
			if (isdigit(param[0])) {
				paramInt = stoi(param);
				move = Util::getMove(plrTeam->getActive()->getMoves()[paramInt-1].getIntName());
			}
			//Make sure the move exists
			else if (Util::checkMove(param) > 0) {
				move = Util::getMove(param);
			}
			else {
				cout << "That move doesn't exist, please try again." << endl;
				return crashAction;
			}

			cout << endl;
			cout << "Here's some info about " << move.getName() << ":" <<endl;
			move.printBattleInfo();
			cout << endl;
		}

		return crashAction;

	}
	//If the player is trying to switch to a differen pokemon or
		//use an attack, check its legality and return a bad action if it fails
	else if (inputs[0] == "SWITCH" || inputs[0] == "USE") {
		if (!checkInputLegality(inputs, outputAction)) {

			return crashAction;
		}
	}
	//Return a bad action if anything else is type
	else {
		//Do nothing if nothing is entered
		if (inputs[0] != "") {
			cout << "That's an invalid command, type \"help\" for help." << endl;
		}

		return crashAction;
	}

	//Check action legality and return a bad action if it fails
	if (!checkActionLegality(outputAction)) {

		return crashAction;
	}

	return *outputAction;
}

//Get player input but the player doesn't need to specify the type of action
TurnAction ActionManager::getPlayerInput(Util::Action action) {
	string line;
	vector<string> inputs;
	TurnAction *outputAction = new TurnAction();

	TurnAction crashAction(Util::Player_1, Util::Other, "NONE");

	//Gets the player's input
	getline(cin, line);

	//Split the input by spaces
	inputs = FileReader::split(line, ' ');

	//if the player input nothing return a bad action
	if (inputs.size() == 0) {
		return crashAction;
	}

	//Format the input to all CAPS
	for (int i = 0; i < inputs.size(); i++) {
		inputs[i] = FileReader::formatName(inputs[i]);
	}

	//If an attack is forced, add that to the inputs and check legality
	if (action == Util::Attack) {
		inputs.insert(inputs.begin(), "USE");

		if (!checkInputLegality(inputs, outputAction)) {

			return crashAction;
		}
	} 
	//If a switch is forced, add that to the inputs and check legality
	else if (action == Util::Switch) {
		inputs.insert(inputs.begin(), "SWITCH");

		if (!checkInputLegality(inputs, outputAction)) {

			return crashAction;
		}
	}

	//Check the action legality and return a bad action if it fails
	if (!checkActionLegality(outputAction)) {

		return crashAction;
	}

	return *outputAction;
}

//Takes a user, team, and parameter
//Performs a switch action
void ActionManager::performSwitch(Util::User user, Team *team, int param) {
	ActivePokemon *active;

	ActiveMove move;
	Move moveInfo;

	string identifier;

	if (user == Util::Player_1) {
		identifier = "You";
	}
	else {
		identifier = "They";
	}

	active = team->getActive();

	//Reset anything that gets lost on switch
	active->switchReset();

	cout << identifier << " withdrew " << active->getName() << " and sent out " << team->getTeam()[param].getName() << "." << endl;

	//Switch out the pokemon
	team->makeActive(param);

	//Update the turn order
	updateTurnOrder();
}

//Takes a user, userTeam, enemy team, and parameter
void ActionManager::performAttack(Util::User user, Team *team, Team *otherTeam, int param) {
	int damage;

	ActivePokemon *active;

	ActiveMove move;
	Move moveInfo;

	int hit;
	int hitChance;
	bool oneHit;

	int numHits;

	string identifier;

	if (user == Util::Player_1) {
		identifier = "You're ";
	}
	else {
		identifier = "The opposing ";
	}

	active = team->getActive();

	//Skips are generally reserved for whena pokemon dies before using an attack
	if (active->decrementVolStatus(Util::Skip)) {
		return;
	}

	//Checks for any relevant status conditions, decrements them and reacts accordingly
	if (active->getStatus() == Util::Asleep) {
		if (active->decrementSleep()) {
			cout << active->getName() << " woke up!" << endl;
		}
		else {
			cout << active->getName() << " is fast asleep!" << endl;
			return;
		}
	}
	else if (active->getStatus() == Util::Frozen) {
		int random = rand() % 5;

		if (random == 0) {
			active->setStatus(Util::Standard);
			cout << active->getName() << " thawed out!" << endl;
		}
		else {
			cout << active->getName() << " is frozen solid!" << endl;
			return;
		}
	}
	//Paralysis is purely random and doesn't rely on decrementing a counter
	else if (active->getStatus() == Util::Paralyzed) {
		int random = rand() % 4;

		if (random == 0) {
			cout << active->getName() << " is paralyzed! It can't move!" << endl;
			return;
		}
	}

	//Can be ignored 
	if (active->getCharge()) {
		return;
	}

	//Makes sure that the move was not disabled
	//This is possible when a faster pokemon disables the move of a slower one
	if (active->decrementDisableTimer()) {
		cout << active->getName() << "'s move is disabled!" << endl;
		return;
	}

	//Checks to see if the user flinched and skips their turn
	if (active->decrementVolStatus(Util::Flinched)) {
		cout << active->getName() << " flinched!" << endl;
		return;
	}

	//If confused decrement and see if the user hit's themselves
	if (active->decrementVolStatus(Util::Confused)) {
		//See if they hit themselves and skip their turn if they do
		if (rand() % 100 < 33) {
			cout << active->getName() << " hit itself in confusion!" << endl;

			//Calculate the damage of hitting yourself
			int damageConf = DamageManager::calcConfusion(*active);

			active->damage(damageConf);
			return;
		}
	}

	//Get the move and move data
	move = active->getMoves()[param];
	moveInfo = Util::getMove(move.getIntName());

	//Determine if the move will hit
	hit = rand() % 100;
	hitChance = moveInfo.getAcc();

	hitChance *= ActivePokemon::calculateAccuracy(active->getStatBoost(Util::AccBoost) - otherTeam->getActive()->getStatBoost(Util::EvasionBoost));

	if (hitChance < 33) {
		hitChance = 33;
	}

	//If the move is a One-Hit KO move calculate the odds of hitting (Its based on level)
	oneHit = MoveEffectManager::activateOHKOAcc(moveInfo,&hitChance, active, otherTeam->getActive());
	
	//Currently not fully implemented
	//MoveEffectManager::activate2TurnMove(moveInfo, active, user);
	
	//Store the most recently used move
	active->setStoredMove(param);

	cout << identifier << active->getName() << " used " << move.getName() << "." << endl;

	//This can be ignored as it's not fully implemented
	//If a user is flying or digging don't let them be hit
	//Allow certain moves to hit through these
	if (otherTeam->getActive()->getFly() || otherTeam->getActive()->getDig()) {
		cout << otherTeam->getActive()->getName() << " avoided the attack." << endl;
		return;
	}

	//See if the move hits
	if (hit < hitChance) {

		//If the move is a One-hit KO move kill the enemy
		if (oneHit) {
			cout << otherTeam->getActive()->getName() << " lost all its health!" << endl;

			otherTeam->getActive()->damagePercent(1);
			return;
		}

		//If the move is a damaging move continue
		if (moveInfo.getMoveType() != Util::Status) {
			//Determine the number of times the mvoe should hit
			numHits = MoveEffectManager::activateMultiHitEffect(moveInfo);

			//Perform the attack that many times
			for (int i = 0; i < numHits; i++) {
				//If the enemy faints stop attacking
				if (otherTeam->getActive()->getHealth() == 0) {
					numHits = i;
					break;
				}

				//Calculate the damage against the enemy
				damage = DamageManager::calcDamage(move, *(active), *(otherTeam->getActive()));

				//Check to see if the move should deal flat damage
				MoveEffectManager::activateFlatDamage(moveInfo, active, &damage);
				//Check to see if the move should deal damage based on the damage it received earlier
				MoveEffectManager::activateCounter(moveInfo, active, &damage);

				//If damage isn't 0
				if (damage > 0) {
					//Print the health loss
					cout << otherTeam->getActive()->getName() << " lost " << damage << " health!" << endl;

					//Damage the enemy
					otherTeam->getActive()->damage(damage);
					
					//If its physical have the enemy record the damage they took
					if (moveInfo.getMoveType() == Util::Physical) {
						otherTeam->getActive()->setStoredDamage(damage);
					}
					//Otherwise reset the damage took
					else {
						otherTeam->getActive()->setStoredDamage(0);
					}

					//Check to see if the user should take any recoil (although this does heal too)
					MoveEffectManager::activateRecoil(moveInfo, active, damage);

					//Activate any additional effects the move may have
					MoveEffectManager::activateDamageEffect(moveInfo, active, otherTeam->getActive());
					//Check if the move is meant to switch the enemy pokemon out
					MoveEffectManager::activateForceSwitch(moveInfo, team, otherTeam);
				}
			}

			//State the number of ties the move hit if more than 1
			if (numHits > 1) {
				cout << "It hit " << numHits << " times!" << endl;
			}
		}
		//If the move is a status move apply the effects
		else {
			MoveEffectManager::activateDamageEffect(moveInfo, active, otherTeam->getActive());
			MoveEffectManager::activateForceSwitch(moveInfo, team, otherTeam);
		}
	}
	//If it misses state it
	else {
		cout << "But it missed..." << endl;
		//Check if anything happens from missing an attack
		MoveEffectManager::activateOnMiss(moveInfo, active);
	}
}

//Takes in a turn action and determines what to do with it
void ActionManager::performAction(TurnAction action) {
	Util::Action actType = action.getAction();
	Util::User user = action.getUser();
	string param = action.getParam();

	Team *team;
	Team *otherTeam;

	int paramInt = stoi(param);

	if (user == Util::Player_1) {
		team = plrTeam;
		otherTeam = enemyTeam;
	}
	else {
		team = enemyTeam;
		otherTeam = plrTeam;
	}

	cout << endl;
	//Either performs a switch or an attack
	if (actType == Util::Switch) {
		performSwitch(user, team, paramInt);
	}
	else {
		performAttack(user, team, otherTeam, paramInt);
	}
}

//Does any clean up duty at the end of turn
void ActionManager::endTurn() {
	Team *team;

	//For each pokemon
	for (int i = 0; i < turnOrder.size(); i++) {
		ActivePokemon *active;
		Util::StatusCond status;

		if (turnOrder[i] == Util::Player_1) {
			team = plrTeam;
		}
		else {
			team = enemyTeam;
		}

		active = team->getActive();
		status = active->getStatus();

		//Reset end of turn related things
		active->setStoredDamage(0);
		active->decrementVolStatus(Util::Flinched);
		active->decrementVolStatus(Util::Skip);

		//If a pokemon is fainted switch them out for a new one
		if (status == Util::Fainted) {
			TurnAction switchAction;

			if (turnOrder[i] == Util::Player_1) {
				if (!plrTeam->checkIfTeamAlive()) {
					return;
				}

				cout << "Your pokemon has fainted, what pokemon will you send out? ";
				switchAction = getPlayerInput(Util::Switch);

				cout << endl;

				while (switchAction.getAction() == Util::Other) {
					cout << "Your pokemon has fainted, what pokemon will you send out? ";
					switchAction = getPlayerInput(Util::Switch);

					cout << endl;
				}

				performAction(switchAction);
			} else {
				if (!enemyTeam->checkIfTeamAlive()) {
					return;
				}
				//AI Module decides

			}
		}
		//Deal damage to any burned pokemon
		else if (status == Util::Burned) {
			active->damagePercent(0.0625);
			cout << active->getName() << " was hurt by its burn!" << endl;
			cout << endl;
		}
		//Deal damage to any poisoned pokemon
		else if (status == Util::Poisoned) {
			active->damagePercent(0.125);
			cout << active->getName() << " was hurt by poison!" << endl;
			cout << endl;
		}
		//Increment poison timer and deal damage to any badly poisoned pokemon
		//Currently no way of making a pokemon badly poisoned
		else if (status == Util::Badly_Poisoned) {
			active->incrementPoison();
			active->damagePercent(active->getPoison()*0.0625);
			cout << active->getName() << " was hurt by poison!" << endl;
			cout << endl;
		}
	}
}

//Go through the action queue and pop and perform each action
//End the turn
void ActionManager::performActions() {
	TurnAction action;
	int size = actionQueue.size();

	for (int i = 0; i < size; i++) {
		action = actionQueue.front();

		performAction(action);

		actionQueue.pop();
	}

	cout << endl;
	endTurn();
}

bool ActionManager::checkTeamLoss() {
	if (!enemyTeam->checkIfTeamAlive()) {
		cout << "Congratulations! You've won!" << endl;
		return true;
	}

	if (!plrTeam->checkIfTeamAlive()) {
		cout << "All your pokemon have fainted! Try again some other time!" << endl;
		return true;
	}

	return false;
}