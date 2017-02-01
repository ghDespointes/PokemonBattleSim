#include "ActionManager.h"



ActionManager::ActionManager()
{
}

ActionManager::ActionManager(Team *plr, Team *enemy) {
	plrTeam = plr;
	enemyTeam = enemy;

	turnOrder.push_back(Util::Player_1);
	turnOrder.push_back(Util::Enemy_1);

	updateTurnOrder();
}


ActionManager::~ActionManager(){
	delete plrTeam;
	delete enemyTeam;
}

void ActionManager::updateTurnOrder() {
	int plrSpeed = plrTeam->getActive()->getStat(Util::Speed);
	int enmSpeed = enemyTeam->getActive()->getStat(Util::Speed);

	plrSpeed *= plrTeam->getActive()->getStatBoost(Util::SpeedBoost);
	enmSpeed *= enemyTeam->getActive()->getStatBoost(Util::SpeedBoost);

	if (plrTeam->getActive()->getStatus() == Util::Paralyzed) {
		plrSpeed /= 2;
	}

	if (enemyTeam->getActive()->getStatus() == Util::Paralyzed) {
		enmSpeed /= 2;
	}
	
	speedTie = false;

	if (plrSpeed > enmSpeed) {
		turnOrder[0] = Util::Player_1;
		turnOrder[1] = Util::Enemy_1;

	} else if(plrSpeed < enmSpeed) {
		turnOrder[1] = Util::Enemy_1;
		turnOrder[0] = Util::Player_1;

	} else {
		speedTie = true;
	}
}

void ActionManager::changeSpeedTie() {
	int rand50 = rand() % 100;

	if (rand50 < 50) {
		Util::User temp = turnOrder[0];
		turnOrder[0] = turnOrder[1];
		turnOrder[1] = temp;
	}
}

bool ActionManager::checkInputLegality(vector<string> input, TurnAction *action) {
	int num;

	if (input[0] == "USE") {
		if (input.size() == 1) {
			cout << "That's an invalid move, please pick another." << endl;
			return false;
		}

		if (!isdigit(input[1][0])) {
			for (int i = 2; i < input.size(); i++) {
				input[1] = input[1] + input[i];
			}
		}
		else {
			num = stoi(input[1]);
			if (num > plrTeam->getActive()->getMoves().size() || num == 0) {
				cout << "Please pick a valid attack" << endl;
				return false;
			}
		}

		action->setUser(Util::Player_1);
		action->setAction(Util::Attack);
		action->setParam(input[1]);

	}
	else if (input[0] == "SWITCH") {
		if (input.size() == 1) {
			cout << "That's an invalid pokemon, please pick another." << endl;
			return false;
		}

		if (!isdigit(input[1][0])) {
			for (int i = 2; i < input.size(); i++) {
				input[1] = input[1] + input[i];
			}
		}
		else {
			num = stoi(input[1]);
			if (num >= plrTeam->getTeam().size() || num == 0) {
				cout << "Please pick a valid pokemon." << endl;
				return false;
			}

			if (plrTeam->getTeam()[num].getStatus() == Util::Fainted) {
				cout << "That pokemon is fainted, please pick another." << endl;
				return false;
			}
		}
		
		action->setUser(Util::Player_1);
		action->setAction(Util::Switch); 
		action->setParam(input[1]);
	}

	return true;
}

bool ActionManager::checkActionLegality(TurnAction* action) {
	Util::User user = action->getUser();
	Util::Action actType = action->getAction();
	string param = action->getParam();

	Team *team;

	bool isInt = isdigit(param[0]);
	int paramInt;

	if (isInt) {
		paramInt = stoi(param);
	}

	if (user == Util::Player_1) {
		team = plrTeam;
	} else {
		team = enemyTeam;
	}

	if (actType == Util::Attack) {
		if (!isInt) {
			if (team->getActive()->findMove(param) == -1) {
				cout << "That move doesn't exist, please try again." << endl;

				return false;
			}

			paramInt = team->getActive()->findMove(param);
		}
		else {
			paramInt -= 1;
		}

		if (team->getActive()->getDisableMove() == paramInt) {
			cout << "That move is disabled, please pick another." << endl;
			return false;
		}

	} else {
		if (!isInt) {
			if (team->getActive()->getIntName() == param) {
				cout << "That pokemon is already out, please try again." << team->getActive()->getIntName() << endl;

				return false;
			}

			if (team->findPoke(param) == -1) {
				cout << "That pokemon isn't on your team, please try again." << endl;

				return false;
			}

			if (team->getTeam()[team->findPoke(param)].getStatus() == Util::Fainted) {
				cout << "That pokemon is fainted, please try again." << endl;

				return false;
			}

			paramInt = team->findPoke(param);
		}
	}

	action->setParam(to_string(paramInt));

	return true;
}

void ActionManager::createActionQueue(TurnAction plrAction, TurnAction enmAction) {
	vector<TurnAction> turnActions;
	TurnAction action;
	string intName;

	Team *team;

	turnActions.push_back(plrAction);
	turnActions.push_back(enmAction);

	if (speedTie) {
		changeSpeedTie();
	}

	for (int i = 0; i < turnOrder.size(); i++) {
		action = turnActions[turnOrder[i]];

		if (action.getAction() == Util::Switch) {
			actionQueue.push(action);
		}
	}

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

TurnAction ActionManager::getPlayerInput() {
	string line;
	vector<string> inputs;
	TurnAction *outputAction = new TurnAction();

	TurnAction crashAction(Util::Player_1, Util::Other, "NONE");
	TurnAction storedMove = plrTeam->getActive()->getStoredAction();

	if (storedMove.getParam() != "CHARGE_MOVE") {
		cout << plrTeam->getActive()->getStoredAction().getParam() << endl;
		plrTeam->getActive()->resetStoredAction();

		return storedMove;
	}

	getline(cin, line);

	inputs = FileReader::split(line, ' ');

	if (inputs.size() == 0) {
		return crashAction;
	}

	for (int i = 0; i < inputs.size(); i++) {
		inputs[i] = FileReader::formatName(inputs[i]);
	}

	if (inputs[0] == "QUIT") {
		return TurnAction(Util::Player_1, Util::Other, "QUIT");
	}
	else if (inputs[0] == "HELP") {
		cout << "Here's some help" << endl;

		return crashAction;

	}
	else if (inputs[0] == "INFO") {
		cout << "Here's some info about" << endl;

		return crashAction;

	}
	else if (inputs[0] == "SWITCH" || inputs[0] == "USE") {
		if (!checkInputLegality(inputs, outputAction)) {

			return crashAction;
		}
	}
	else {
		if (inputs[0] != "") {
			cout << "That's an invalid command, type \"help\" for help." << endl;
		}

		return crashAction;
	}

	if (!checkActionLegality(outputAction)) {

		return crashAction;
	}

	return *outputAction;
}

TurnAction ActionManager::getPlayerInput(Util::Action action) {
	string line;
	vector<string> inputs;
	TurnAction *outputAction = new TurnAction();

	TurnAction crashAction(Util::Player_1, Util::Other, "NONE");

	getline(cin, line);

	inputs = FileReader::split(line, ' ');

	if (inputs.size() == 0) {
		return crashAction;
	}

	for (int i = 0; i < inputs.size(); i++) {
		inputs[i] = FileReader::formatName(inputs[i]);
	}

	if (action == Util::Attack) {
		inputs.insert(inputs.begin(), "USE");

		if (!checkInputLegality(inputs, outputAction)) {

			return crashAction;
		}
	} else if (action == Util::Switch) {
		inputs.insert(inputs.begin(), "SWITCH");

		if (!checkInputLegality(inputs, outputAction)) {

			return crashAction;
		}
	} else {
		if (inputs[0] != "") {
			cout << "That's an invalid command, type \"help\" for help." << endl;
		}

		return crashAction;
	}

	if (!checkActionLegality(outputAction)) {

		return crashAction;
	}

	return *outputAction;
}

void ActionManager::performSwitch(Util::User user, Team *team, int param) {
	ActivePokemon *active;

	ActiveMove move;
	Move moveInfo;

	int hit;

	string identifier;

	if (user == Util::Player_1) {
		identifier = "You";
	}
	else {
		identifier = "They";
	}

	active = team->getActive();
	
	active->setStoredMove(-1);

	cout << endl;
	cout << identifier << " withdrew " << active->getName() << " and sent out " << team->getTeam()[param].getName() << "." << endl;

	team->makeActive(param);

	updateTurnOrder();
}

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
	else if (active->getStatus() == Util::Paralyzed) {
		int random = rand() % 4;

		if (random == 0) {
			cout << active->getName() << " is paralyzed! It can't move!" << endl;
			return;
		}
	}

	if (active->getCharge()) {
		return;
	}

	if (active->decrementDisableTimer()) {
		cout << active->getName() << "'s move is disabled!" << endl;
		return;
	}

	if (active->decrementVolStatus(Util::Skip)) {
		return;
	}

	if (active->decrementVolStatus(Util::Flinched)) {
		cout << active->getName() << " flinched!" << endl;
		return;
	}

	if (active->decrementVolStatus(Util::Confused)) {
		cout << active->getName() << " hit itself in confusion!" << endl;

		int damageConf = DamageManager::calcConfusion(*active);

		cout << damageConf << endl;

		active->damage(damageConf);
		return;
	}

	move = active->getMoves()[param];
	moveInfo = Util::getMove(move.getIntName());

	hit = rand() % 100;
	hitChance = moveInfo.getAcc();

	oneHit = MoveEffectManager::activateOHKOAcc(moveInfo,&hitChance, active, otherTeam->getActive());
	MoveEffectManager::activate2TurnMove(moveInfo, active, user);

	active->setStoredMove(param);

	cout << endl;
	cout << identifier << active->getName() << " used " << move.getName() << "." << endl;

	//Allow certain moves to hit through these
	if (otherTeam->getActive()->getFly() || otherTeam->getActive()->getDig()) {
		cout << otherTeam->getActive()->getName() << " avoided the attack." << endl;
		return;
	}

	if (hit < hitChance) {

		if (oneHit) {
			cout << otherTeam->getActive()->getName() << " lost all its health!" << endl;

			otherTeam->getActive()->damagePercent(1);
			return;
		}

		if (moveInfo.getMoveType() != Util::Status) {
			numHits = MoveEffectManager::activateMultiHitEffect(moveInfo);

			for (int i = 0; i < numHits; i++) {
				if (otherTeam->getActive()->getHealth() == 0) {
					numHits = i;
					break;
				}

				damage = DamageManager::calcDamage(move, *(active), *(otherTeam->getActive()));

				MoveEffectManager::activateFlatDamage(moveInfo, active, &damage);
				MoveEffectManager::activateCounter(moveInfo, active, &damage);

				if (damage > 0) {
					cout << otherTeam->getActive()->getName() << " lost " << damage << " health!" << endl;

					otherTeam->getActive()->damage(damage);
					
					if (moveInfo.getMoveType() == Util::Physical) {
						otherTeam->getActive()->setStoredDamage(damage);
					}
					else {
						otherTeam->getActive()->setStoredDamage(0);
					}

					MoveEffectManager::activateRecoil(moveInfo, active, damage);
				}

				MoveEffectManager::activateDamageEffect(moveInfo, active, otherTeam->getActive());
				MoveEffectManager::activateForceSwitch(moveInfo, team, otherTeam);
			}

			if (numHits > 1) {
				cout << "It hit " << numHits << " times!" << endl;
			}
		}
		else {
			MoveEffectManager::activateDamageEffect(moveInfo, active, otherTeam->getActive());
			MoveEffectManager::activateForceSwitch(moveInfo, team, otherTeam);
		}
	}
	else {
		cout << "But it missed..." << endl;
		MoveEffectManager::activateOnMiss(moveInfo, active);
	}
}

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

	if (actType == Util::Switch) {
		performSwitch(user, team, paramInt);
	}
	else {
		performAttack(user, team, otherTeam, paramInt);
	}
}

void ActionManager::endTurn() {
	Team *team;

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

		active->setStoredDamage(0);
		active->decrementVolStatus(Util::Flinched);
		active->decrementVolStatus(Util::Skip);

		if (status == Util::Fainted) {
			TurnAction switchAction;

			if (turnOrder[i] == Util::Player_1) {

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
				//AI Module decides

			}
		}
		else if (status == Util::Burned) {
			active->damagePercent(0.0625);
			cout << active->getName() << " was hurt by its burn!" << endl;

		}
		else if (status == Util::Poisoned) {
			active->damagePercent(0.125);
			cout << active->getName() << " was hurt by poison!" << endl;
		}
		else if (status == Util::Badly_Poisoned) {
			active->damagePercent(active->getPoison()*0.0625);
			cout << active->getName() << " was hurt by poison!" << endl;
		}
	}
}

void ActionManager::performActions() {
	TurnAction action;
	int size = actionQueue.size();

	for (int i = 0; i < size; i++) {
		action = actionQueue.front();

		performAction(action);

		cout << endl;

		actionQueue.pop();
	}

	endTurn();
}