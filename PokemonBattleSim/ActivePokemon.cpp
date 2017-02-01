#include "ActivePokemon.h"



ActivePokemon::ActivePokemon(){
	//Dummy Pokemon

	name = "NONE";
	internalName = "NONE";
	ID = -1;

	item = "NONE";
	ability = "NONE";

	lvl = 0;
	stats = {0,0,0,0,0,0};
	evs = {0,0,0,0,0,0};
	ivs = {0,0,0,0,0,0};
	statBoosts = {0,0,0,0,0,0,0,0};
	volStatus = { 0,0,0,0 };
}

ActivePokemon::ActivePokemon(vector<string> input){
	name = input[Util::ACTIVE_MON_NAME];
	internalName = input[Util::ACTIVE_MON_INT_NAME];

	Pokemon pokeInfo = Util::pokeHash.at(internalName);

	ID = pokeInfo.getID();

	lvl = stoi(input[Util::ACTIVE_MON_LEVEL]);

	nature = input[Util::ACTIVE_MON_NATURE];
	item = input[Util::ACTIVE_MON_ITEM];
	ability = input[Util::ACTIVE_MON_ABILITY];

	evs = { stoi(input[Util::ACTIVE_MON_EV_HP]), 
			stoi(input[Util::ACTIVE_MON_EV_ATT]), 
			stoi(input[Util::ACTIVE_MON_EV_DEF]), 
			stoi(input[Util::ACTIVE_MON_EV_SP_ATT]), 
			stoi(input[Util::ACTIVE_MON_EV_SP_DEF]), 
			stoi(input[Util::ACTIVE_MON_EV_SPEED])};

	ivs = { stoi(input[Util::ACTIVE_MON_IV_HP]),
			stoi(input[Util::ACTIVE_MON_IV_ATT]),
			stoi(input[Util::ACTIVE_MON_IV_DEF]),
			stoi(input[Util::ACTIVE_MON_IV_SP_ATT]),
			stoi(input[Util::ACTIVE_MON_IV_SP_DEF]),
			stoi(input[Util::ACTIVE_MON_IV_SPEED])};

	//Make sure ev and iv rules aren't violated
	fixEvs();
	fixIVs();

	status = Util::Standard;

	//Debugging purpses
	/*for (int i = 0; i < evs.size(); i++) {
		cout << evs[i] << endl;
	}

	for (int i = 0; i < ivs.size(); i++) {
		cout << ivs[i] << endl;
	}*/

	stats = { 0,0,0,0,0,0 };

	//Calculate stats based on level, evs, ivs, and nature
	calculateStats(pokeInfo);
	health = stats[0];

	statBoosts = { 0,0,0,0,0,0,0,0 };
	volStatus = { 0,0,0,0,0 };

	storedAction = TurnAction(Util::Player_1, Util::Attack, "CHARGE_MOVE");

	for (int i = 0; i < 4; i++) {
		if (Util::ACTIVE_MON_MOVE_LOC + i >= input.size()) {
			break;
		}

		ActiveMove newMove(input[Util::ACTIVE_MON_MOVE_LOC + i]);
		moves.push_back(newMove);
	}
}

ActivePokemon::~ActivePokemon(){

}

void ActivePokemon::fixEvs() {
	int total = 0;

	for (int i = 0; i < evs.size(); i++) {
		if (total >= 510) {
			evs[i] = 0;

		} else if (total + evs[i] > 510) {
			int dif = 510 - total;

			total = 510;
			evs[i] = dif;

		} else if (evs[i] > 252) {
			evs[i] = 252;
			total += 252;

		} else if (evs[i] < 0) {
			evs[i] = 0;

		} else {
			total += evs[i];

		}
	}
}

void ActivePokemon::fixIVs() {
	for (int i = 0; i < evs.size(); i++) {
		if (ivs[i] > 31) {
			ivs[i] = 31;
		} else if (ivs[i] < 0) {
			ivs[i] = 0;
		}
	}
}

void ActivePokemon::calculateStats(Pokemon pokeInfo) {
	for (int i = 0; i < 6; i++) {
		int base = pokeInfo.getStat(Util::StatType(i));

		int evCalc = evs[i]/4;
		int statCalc = (2 * base + ivs[i] + evCalc) * lvl / 100;

		int addition = 5;
		
		if (i == 0) {
			addition = lvl + 10;
		} 

		stats[i] = statCalc + addition;
	}

	vector<double> natureAffect = Util::getNatureFromString(nature);

	for (int i = 0; i < natureAffect.size(); i++) {
		stats[i + 1] *= natureAffect[i];
	}
	
	//Debug check stats
	/*for (int i = 0; i < stats.size(); i++) {
		cout << stats[i] << endl << endl;
	}*/
}

string ActivePokemon::getName() {
	return name;
}

string ActivePokemon::getIntName() {
	return internalName;
}

int ActivePokemon::getID() {
	return ID;
}

int ActivePokemon::getLevel() {
	return lvl;
}

string ActivePokemon::getItem() {
	return item;
}

int ActivePokemon::getStat(Util::StatType stat) {
	return stats[stat];
}

int ActivePokemon::getEV(Util::StatType stat) {
	return evs[stat];
}

int ActivePokemon::getIV(Util::StatType stat) {
	return ivs[stat];
}

void ActivePokemon::switchReset() {
	resetStatBoost();
	resetVolStatus();
	storedAction = TurnAction();
}

double ActivePokemon::getStatBoost(Util::StatBoost stat) {
	if (stat == Util::AccBoost || stat == Util::EvasionBoost) {
		if (statBoosts[stat] == -6) {
			return 3.0/9.0;
		}
		else if (statBoosts[stat] == -5) {
			return 3.0/8.0;
		}
		else if (statBoosts[stat] == -4) {
			return 3.0/7.0;
		}
		else if (statBoosts[stat] == -3) {
			return 3.0/6.0;
		}
		else if (statBoosts[stat] == -2) {
			return 3.0/5.0;
		}
		else if (statBoosts[stat] == -1) {
			return 3.0/4.0;
		}
		else if (statBoosts[stat] == 0) {
			return 1;
		}
		else if (statBoosts[stat] == 1) {
			return 4.0/3.0;
		}
		else if (statBoosts[stat] == 2) {
			return 5.0/3.0;
		}
		else if (statBoosts[stat] == 3) {
			return 2;
		}
		else if (statBoosts[stat] == 4) {
			return 7.0/3.0;
		}
		else if (statBoosts[stat] == 5) {
			return 8.0/3.0;
		}
		else if (statBoosts[stat] == 6) {
			return 3;
		}
	}
	else if (stat == Util::CritBoost) {
		if (statBoosts[stat] <= 0) {
			return 1.0/16.0;
		}
		else if (statBoosts[stat] == 1) {
			return 1.0/8.0;
		}
		else if (statBoosts[stat] == 2) {
			return 1.0/2.0;
		}
		else if (statBoosts[stat] >= 3) {
			return 1;
		}
	}
	else {
		if (statBoosts[stat] == -6) {
			return 2.0 / 8.0;
		}
		else if (statBoosts[stat] == -5) {
			return 2.0 / 7.0;
		}
		else if (statBoosts[stat] == -4) {
			return 2.0 / 6.0;
		}
		else if (statBoosts[stat] == -3) {
			return 2.0 / 5.0;
		}
		else if (statBoosts[stat] == -2) {
			return 2.0 / 4.0;
		}
		else if (statBoosts[stat] == -1) {
			return 2.0 / 3.0;
		}
		else if (statBoosts[stat] == 0) {
			return 1;
		}
		else if (statBoosts[stat] == 1) {
			return 3.0/2.0;
		}
		else if (statBoosts[stat] == 2) {
			return 2;
		}
		else if (statBoosts[stat] == 3) {
			return 5.0 / 2.0;
		}
		else if (statBoosts[stat] == 4) {
			return 3.0;
		}
		else if (statBoosts[stat] == 5) {
			return 7.0 / 2.0;
		}
		else if (statBoosts[stat] == 6) {
			return 4.0;
		}
	}
}

void ActivePokemon::changeStatBoost(Util::StatBoost stat, int quant) {
	if (statBoosts[stat] == 6) {
		cout << name << "'s STAT " << "won't increase anymore" << endl;

		return;
	}
	else if (statBoosts[stat] == -6) {
		cout << name << "'s STAT " << "won't decrease anymore" << endl;

		return;
	}
	
	statBoosts[stat] += quant;

	if (statBoosts[stat] > 6) {
		statBoosts[stat] = 6;
	}
	else if (statBoosts[stat] < -6) {
		statBoosts[stat] = -6;
	}

	if (quant == 3) {
		cout << name << "'s STAT "<< "rose drastically!" << endl;
	}
	else if (quant == 2) {
		cout << name << "'s STAT " << "sharply rose!" << endl;
	}
	else if (quant == 1) {
		cout << name << "'s STAT " << "rose!" << endl;
	}
	else if (quant == -1) {
		cout << name << "'s STAT " << "fell!" << endl;
	}
	else if (quant == -2) {
		cout << name << "'s STAT " << "sharply fell!" << endl;
	}
	else if (quant == -3) {
		cout << name << "'s STAT " << "severly fell!" << endl;
	}
}

void ActivePokemon::resetStatBoost() {
	for (int i = 0; i < statBoosts.size(); i++) {
		statBoosts[i] = 0;
	}
}

int ActivePokemon::getHealth() {
	return health;
}

void ActivePokemon::setHealth(int newhealth) {
	health = newhealth;
}

void ActivePokemon::damagePercent(double percent) {
	damage(stats[0]*percent);
}

void ActivePokemon::damage(int ammount) {
	health -= ammount;

	if (health <= 0) {
		health = 0;

		status = Util::Fainted;

		setVolStatus(Util::Skip);
		
		cout << name << " has fainted." << endl;
	}
}

void ActivePokemon::healPercent(double percent) {
	heal(stats[0]*percent);
}

void ActivePokemon::heal(int ammount) {
	health += ammount;

	if (health > stats[0]) {
		health = stats[0];
	}
}

void ActivePokemon::setStatus(Util::StatusCond newStatus) {
	status = newStatus;

	if (newStatus == Util::Asleep) {
		resetSleep();
	}
}

Util::StatusCond ActivePokemon::getStatus() {
	return status;
}

void ActivePokemon::setVolStatus(Util::VolStatus volStat) {
	if (volStat == Util::Bound) {
		volStatus[volStat]  = rand() % 2 + 4;
	}
	else if (volStat == Util::Confused) {
		volStatus[volStat] = rand() % 4 + 1;
	}
	else {
		volStatus[volStat] = 1;
	}
}

bool ActivePokemon::decrementVolStatus(Util::VolStatus volStat) {
	volStatus[volStat]--;

	if (volStatus[volStat] < 0) {
		volStatus[volStat] = 0;

		return false;
	}
	else if (volStatus[volStat] == 0 && volStat == Util::Confused) {
		cout << endl;
		cout << name << " snapped out of its confusion!" << endl;

		return false;
	}

	return true;
}

int ActivePokemon::getVolStatus(Util::VolStatus volStat) {
	return volStatus[volStat];
}

void ActivePokemon::resetVolStatus() {
	for (int i = 0; i < volStatus.size(); i++) {
		volStatus[i] = 0;
	}
}

void ActivePokemon::incrementPoison() {
	poisonTimer++;

	if (poisonTimer > 15) {
		poisonTimer = 15;
	}
}

void ActivePokemon::resetPoison() {
	poisonTimer = 0;
}

int ActivePokemon::getPoison() {
	return poisonTimer;
}

bool ActivePokemon::decrementSleep() {
	sleepTimer--;

	if (sleepTimer <= 0) {
		sleepTimer = 0;

		status = Util::Standard;
		return true;
	}

	return false;
}

void ActivePokemon::resetSleep() {
	sleepTimer = (rand() % 3) + 1;
}

int ActivePokemon::getSleep() {
	return sleepTimer;
}

bool ActivePokemon::getCharge() {
	return chargingMove;
}

bool ActivePokemon::getFly() {
	return isFly;
}

bool ActivePokemon::getDig() {
	return isDig;
}

TurnAction ActivePokemon::getStoredAction() {
	return storedAction;
}

void ActivePokemon::setCharge(bool input) {
	chargingMove = input;
}

void ActivePokemon::setFly(bool input) {
	isFly = input;
}

void ActivePokemon::setDig(bool input) {
	isDig = input;
}

void ActivePokemon::setStoredAction(TurnAction action) {
	storedAction = action;
}

void ActivePokemon::resetStoredAction() {
	storedAction = storedAction = TurnAction(Util::Player_1, Util::Attack, "CHARGE_MOVE");
}

void ActivePokemon::setStoredMove(int move) {
	storedMove = move;
}

int ActivePokemon::getStoredMove() {
	return storedMove;
}

void ActivePokemon::setStoredDamage(int dam) {
	storedDamage = dam;
}

int ActivePokemon::getStoredDamage() {
	return storedDamage;
}

void ActivePokemon::setDisable(int move) {
	disabledMove = move;
	disableTimer = 5;
}

int ActivePokemon::getDisableMove() {
	return disabledMove;
}

bool ActivePokemon::decrementDisableTimer() {
	disableTimer--;

	if (disableTimer == 0) {
		cout << moves[disabledMove].getName() << " is no longer disabled!" << endl;

		disabledMove = -1;

		return false;
	}
	else if (disableTimer < 0){
		return false;
	}

	return true;
}

int ActivePokemon::findMove(string name) {
	for (int i = 0; i < moves.size(); i++) {
		if (moves[i].getIntName() == name) {
			return i;
		}
	}

	return -1;
}

vector<ActiveMove> ActivePokemon::getMoves() {
	return moves;
}

void ActivePokemon::print() {
	cout << "Name: " << name << endl;
	cout << "Internal Name: " << internalName << endl;
	cout << "ID: " << ID << endl;
	cout << "Level: " << lvl << endl;
	cout << "Item: " << item << endl;
	cout << "Ability: " << ability << endl;
	cout << "Nature: " << nature << endl;

	cout << endl;

	cout << "EVs" << endl;
	cout << "HP:\t" << evs[0] << endl;
	cout << "Att:\t" << evs[1] << endl;
	cout << "Def:\t" << evs[2] << endl;
	cout << "Sp Att:\t" << evs[3] << endl;
	cout << "Sp Def:\t" << evs[4] << endl;
	cout << "Speed:\t" << evs[5] << endl;

	cout << endl;

	cout << "IVs" << endl;
	cout << "HP:\t" << ivs[0] << endl;
	cout << "Att:\t" << ivs[1] << endl;
	cout << "Def:\t" << ivs[2] << endl;
	cout << "Sp Att:\t" << ivs[3] << endl;
	cout << "Sp Def:\t" << ivs[4] << endl;
	cout << "Speed:\t" << ivs[5] << endl;

	cout << endl;

	cout << "Stats" << endl;
	cout << "HP:\t" << stats[0] << endl;
	cout << "Att:\t" << stats[1] << endl;
	cout << "Def:\t" << stats[2] << endl;
	cout << "Sp Att:\t" << stats[3] << endl;
	cout << "Sp Def:\t" << stats[4] << endl;
	cout << "Speed:\t" << stats[5] << endl;

	cout << "Status:\t" << status << endl;

	cout << endl << "Moves" << endl;

	for (int i = 0; i < moves.size(); i++) {
		moves[i].print();
	}
}

void ActivePokemon::printBattleInfo() {
	double calcHealth = (int)((double)(health * 1000) / (double)getStat(Util::HP));
	string statusCond = Util::getStatusStringFromEnum(status);

	if (status == Util::Standard) {
		statusCond = "";
	}

	calcHealth = (calcHealth / 10);

	cout << "HP: " << calcHealth << "%\t" << statusCond << endl;
	cout << name << "\t";
	cout << "Lvl: " << lvl << endl;

	vector<Util::PokeType> types = Util::getPoke(internalName).getType();

	cout << Util::getPokeTypeStringFromEnum(types[0]) << " ";

	if (types[1] != Util::NONE) {
		cout << Util::getPokeTypeStringFromEnum(types[1]);
	}

	cout << endl;

	cout << "Item: " << item << endl;
	cout << "Ability: " << ability << endl;

	cout << endl;

	cout << "Moves:" << endl;
	//MAKE MOVES APPEAR SIDE BY SIDE
	for (int i = 0; i < moves.size(); i++) {
		cout << "[" << i+1 << "]" << endl;
		moves[i].printBattleInfo();
	}
}

void ActivePokemon::printBasicInfo() {
	double calcHealth = (int)((double)(health * 1000) / (double)getStat(Util::HP));
	string statusCond = Util::getStatusStringFromEnum(status);

	if (status == Util::Standard) {
		statusCond = "";
	}

	calcHealth = (calcHealth / 10);

	cout << "HP: " << calcHealth << "%\t" << statusCond << endl;
	cout << name << "\t" << item << "\t" << ability << endl;

	vector<Util::PokeType> types = Util::getPoke(internalName).getType();

	cout << Util::getPokeTypeStringFromEnum(types[0]) << " ";

	if (types[1] != Util::NONE) {
		cout << Util::getPokeTypeStringFromEnum(types[1]);
	}

	cout << endl;

	for (int i = 0; i < moves.size(); i++) {
		cout << moves[i].getName() << "\t";

		if (i == 1) { cout << endl; }
	}

	cout << endl;
}

//Show knowledge gained by player (Likes moves)
void ActivePokemon::printEnemyInfo() {
	double calcHealth = (int)((double)(health * 1000) / (double)getStat(Util::HP));
	string statusCond = Util::getStatusStringFromEnum(status);

	if (status == Util::Standard) {
		statusCond = "";
	}

	calcHealth = (calcHealth / 10);

	cout << "HP: " << calcHealth << "%\t" << statusCond << endl;
	cout << name << endl;

	vector<Util::PokeType> types = Util::getPoke(internalName).getType();

	cout << Util::getPokeTypeStringFromEnum(types[0]) << " ";

	if (types[1] != Util::NONE) {
		cout << Util::getPokeTypeStringFromEnum(types[1]);
	}

	cout << endl;
}

void ActivePokemon::printEnemyBasicInfo() {
	double calcHealth = (int)((double)(health * 1000) / (double)getStat(Util::HP));
	string statusCond = Util::getStatusStringFromEnum(status);

	if (status == Util::Standard) {
		statusCond = "";
	}

	calcHealth = (calcHealth / 10);

	cout << "HP: " << calcHealth << "%\t" << statusCond << endl;
	cout << name << endl;

	vector<Util::PokeType> types = Util::getPoke(internalName).getType();

	cout << Util::getPokeTypeStringFromEnum(types[0]) << " ";

	if (types[1] != Util::NONE) {
		cout << Util::getPokeTypeStringFromEnum(types[1]);
	}

	cout << endl;
}