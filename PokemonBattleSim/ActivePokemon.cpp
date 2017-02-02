#include "ActivePokemon.h"


//Dummy Pokemon
ActivePokemon::ActivePokemon(){
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

//Actual Constructor
//Takes in vector of string fro FileReader to produce a pokemon
ActivePokemon::ActivePokemon(vector<string> input){
	//Fairly straightforward of putting values where they belong
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

	//Start with no status conditions
	status = Util::Standard;

	//Debugging purpses
	/*for (int i = 0; i < evs.size(); i++) {
		cout << evs[i] << endl;
	}

	for (int i = 0; i < ivs.size(); i++) {
		cout << ivs[i] << endl;
	}*/

	//Start with 0'd out stats
	stats = { 0,0,0,0,0,0 };

	//Calculate stats based on level, evs, ivs, and nature
	calculateStats(pokeInfo);
	health = stats[0];

	//Start with no modifiers or conditions
	statBoosts = { 0,0,0,0,0,0,0,0 };
	volStatus = { 0,0,0,0,0 };

	//Can be ignored for now
	storedAction = TurnAction(Util::Player_1, Util::Attack, "CHARGE_MOVE");

	//Add between 0-4 moves
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

//Make sure the total cap of 510 isn't exceeded
//make sure the individual cap of 252 isn't exceeded either

//If any goes over it'll be taken into account and corrected
//Excess points are ignored
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

//Simply checks if between 0-31 and clamps it
void ActivePokemon::fixIVs() {
	for (int i = 0; i < evs.size(); i++) {
		if (ivs[i] > 31) {
			ivs[i] = 31;
		} else if (ivs[i] < 0) {
			ivs[i] = 0;
		}
	}
}

//Following a formula from the pokemon games the stats are calculated
void ActivePokemon::calculateStats(Pokemon pokeInfo) {
	for (int i = 0; i < 6; i++) {
		int base = pokeInfo.getStat(Util::StatType(i));

		int evCalc = evs[i]/4;
		int statCalc = (2 * base + ivs[i] + evCalc) * lvl / 100;

		int addition = 5;
		
		//HP is calculated different than the others
		if (i == 0) {
			addition = lvl + 10;
		} 

		stats[i] = statCalc + addition;
	}

	//Have the nature increase or decrease the correspnding stats by 10%
	vector<double> natureAffect = Util::getNatureFromString(nature);

	for (int i = 0; i < natureAffect.size(); i++) {
		stats[i + 1] *= natureAffect[i];
	}
	
	//Debug check stats
	/*for (int i = 0; i < stats.size(); i++) {
		cout << stats[i] << endl << endl;
	}*/
}

//Get name
string ActivePokemon::getName() {
	return name;
}

//Get internal Name
string ActivePokemon::getIntName() {
	return internalName;
}

//Get ID
int ActivePokemon::getID() {
	return ID;
}

//Get level
int ActivePokemon::getLevel() {
	return lvl;
}


//Get item Name
string ActivePokemon::getItem() {
	return item;
}

//Takes in a stat type
//Returns the stat of that type
int ActivePokemon::getStat(Util::StatType stat) {
	return stats[stat];
}

//Takes in a stat type
//Returns the EV for that stat
int ActivePokemon::getEV(Util::StatType stat) {
	return evs[stat];
}

//Takes in a stat type
//Returns the IV for that stat
int ActivePokemon::getIV(Util::StatType stat) {
	return ivs[stat];
}

//Takes a type
//Return true if the pokemon is that type
bool ActivePokemon::isType(Util::PokeType type) {
	vector<Util::PokeType> types = Util::getPoke(internalName).getType();

	for (int i = 0; i < types.size(); i++) {
		if (types[i] == type) {
			return true;
		}
	}
	return false;
}

//Resets any value when a switch occurs
void ActivePokemon::switchReset() {
	resetStatBoost();
	resetVolStatus();
	storedAction = TurnAction();
	setStoredMove(-1);
	setStoredDamage(0);
}

//Takes statBoost type
//Returns double modifier
//Stat boosts are split into three kinds
	//Main Stats (HP, Attack, Defense, SpAttack, SpDefense, Speed)
	//Evasion Stats (Accureacy, Evasion)
	//Critical Hit Chance
//Each is different depending on level
double ActivePokemon::getStatBoost(Util::StatBoost stat) {
	//Evasion and Accuracy are added together later so return the number and calculate it later
	if (stat == Util::AccBoost || stat == Util::EvasionBoost) {
		return statBoosts[stat];
	
	}
	//Crit chance ranges from 1/16 - 100%
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
	//Main stats range from 1/4 - 4
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

//Takes acuracy - evasion
//Returns final accuracy as a double
double ActivePokemon::calculateAccuracy(int level) {
	if (level <= -6) {
		return 3.0 / 9.0;
	}
	else if (level == -5) {
		return 3.0 / 8.0;
	}
	else if (level == -4) {
		return 3.0 / 7.0;
	}
	else if (level == -3) {
		return 3.0 / 6.0;
	}
	else if (level == -2) {
		return 3.0 / 5.0;
	}
	else if (level == -1) {
		return 3.0 / 4.0;
	}
	else if (level == 0) {
		return 1;
	}
	else if (level == 1) {
		return 4.0 / 3.0;
	}
	else if (level == 2) {
		return 5.0 / 3.0;
	}
	else if (level == 3) {
		return 2;
	}
	else if (level == 4) {
		return 7.0 / 3.0;
	}
	else if (level == 5) {
		return 8.0 / 3.0;
	}
	else if (level >= 6) {
		return 3;
	}

	return 1;
}

//Takes statBoost type and ammount of change
//StatBoosts generally range from -6 - 6
	//Crit chance ranges from 0 - 6
void ActivePokemon::changeStatBoost(Util::StatBoost stat, int quant) {
	if (statBoosts[stat] == 6 && quant > 0) {
		cout << name << "'s STAT " << "won't increase anymore" << endl;

		return;
	}
	else if (statBoosts[stat] == -6 && quant < 0) {
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

	cout << name << "'s " << Util::getStatBoostStringFromEnum(stat);

	if (quant == 3) {
		cout << " rose drastically!" << endl;
	}
	else if (quant == 2) {
		cout << " sharply rose!" << endl;
	}
	else if (quant == 1) {
		cout << " rose!" << endl;
	}
	else if (quant == -1) {
		cout << " fell!" << endl;
	}
	else if (quant == -2) {
		cout << " sharply fell!" << endl;
	}
	else if (quant == -3) {
		cout << " severly fell!" << endl;
	}
}

//Returns stat modifiers back to 0
void ActivePokemon::resetStatBoost() {
	for (int i = 0; i < statBoosts.size(); i++) {
		statBoosts[i] = 0;
	}
}

//Returns current health
int ActivePokemon::getHealth() {
	return health;
}

//Sets current health
void ActivePokemon::setHealth(int newhealth) {
	health = newhealth;
}

//Takes double of percentage of macHealth
//deals damage equal to a percentage of maxHealth
void ActivePokemon::damagePercent(double percent) {
	damage(stats[0]*percent);
}

//Takes damage ammount
//Deals that much damage and clamps the health above 0
void ActivePokemon::damage(int ammount) {
	health -= ammount;

	if (health <= 0) {
		health = 0;

		status = Util::Fainted;

		setVolStatus(Util::Skip);
		
		cout << name << " has fainted." << endl;
	}
}

//Takes double of percentage of macHealth
//heals equal to a percentage of maxHealth
void ActivePokemon::healPercent(double percent) {
	heal(stats[0]*percent);
}

//Takes heal ammount
//Heals that much and clamps the health below maxHealth
void ActivePokemon::heal(int ammount) {
	health += ammount;

	if (health > stats[0]) {
		health = stats[0];
	}
}

//Takes status condition
//Sets pokemon status
	//If put to sleep pokemon's sleep timer is reset
void ActivePokemon::setStatus(Util::StatusCond newStatus) {
	status = newStatus;

	if (newStatus == Util::Asleep) {
		resetSleep();
	}
}

//Returns status
Util::StatusCond ActivePokemon::getStatus() {
	return status;
}

//Takes volatile Status
//Sets that volatile stat
	//Number depends on how many turns it lasts
void ActivePokemon::setVolStatus(Util::VolStatus volStat) {
	//Bound status isn't implemented, but will last between 4-5 turns
	if (volStat == Util::Bound) {
		volStatus[volStat]  = rand() % 2 + 4;
	}
	//Confusion lasts between 1-4 turns
	else if (volStat == Util::Confused) {
		volStatus[volStat] = rand() % 4 + 1;
	}
	//Other volatile status conditions are bools
	else {
		volStatus[volStat] = 1;
	}
}

//Takes volatile stat type
//Decrements the value and returns true if its still has that status
	//returns false if cured or not afflicted
bool ActivePokemon::decrementVolStatus(Util::VolStatus volStat) {
	volStatus[volStat]--;

	//Clamp it to positive numbers
		//This implies they weren't afflicted so return false;
	if (volStatus[volStat] < 0) {
		volStatus[volStat] = 0;

		return false;
	}
	// When cured of status, state it
	else if (volStatus[volStat] == 0 && volStat == Util::Confused) {
		cout << endl;
		cout << name << " snapped out of its confusion!" << endl;

		return false;
	}

	return true;
}

//Takes volatile status type
//returns current value for that type
int ActivePokemon::getVolStatus(Util::VolStatus volStat) {
	return volStatus[volStat];
}

//Resets all volatile statuses to 0
void ActivePokemon::resetVolStatus() {
	for (int i = 0; i < volStatus.size(); i++) {
		volStatus[i] = 0;
	}
}

//Increase poison counter by one clamping it at 15
void ActivePokemon::incrementPoison() {
	poisonTimer++;

	if (poisonTimer > 15) {
		poisonTimer = 15;
	}
}

//Reset Poison
void ActivePokemon::resetPoison() {
	poisonTimer = 0;
}

//Return poison counter
int ActivePokemon::getPoison() {
	return poisonTimer;
}

//Decrease sleep timer
//Return true awoken
//return false if still asleep
bool ActivePokemon::decrementSleep() {
	sleepTimer--;
	cout << sleepTimer << endl;

	if (sleepTimer <= 0) {
		sleepTimer = 0;

		status = Util::Standard;
		return true;
	}

	return false;
}

//Set sleep to last between 1-3 turns
void ActivePokemon::resetSleep() {
	sleepTimer = (rand() % 3) + 1;
	cout << sleepTimer;
}

//Return sleep timer
int ActivePokemon::getSleep() {
	return sleepTimer;
}

//
//
//
//Following not being used
//
//
//

//Returns state of charging move
bool ActivePokemon::getCharge() {
	return chargingMove;
}

//Return fly Flag
bool ActivePokemon::getFly() {
	return isFly;
}

//Returns dig Flag
bool ActivePokemon::getDig() {
	return isDig;
}

//Returns stored action
TurnAction ActivePokemon::getStoredAction() {
	return storedAction;
}

//Sets chargeMove flag
void ActivePokemon::setCharge(bool input) {
	chargingMove = input;
}

//Sets fly flag
void ActivePokemon::setFly(bool input) {
	isFly = input;
}

//sets dig flag
void ActivePokemon::setDig(bool input) {
	isDig = input;
}

//Sets stored Action to input action
void ActivePokemon::setStoredAction(TurnAction action) {
	storedAction = action;
}

//Resets stored action to a null state
void ActivePokemon::resetStoredAction() {
	storedAction = storedAction = TurnAction(Util::Player_1, Util::Attack, "CHARGE_MOVE");
}

//
//
//
//Above not being used
//
//
//

//set stored move
void ActivePokemon::setStoredMove(int move) {
	storedMove = move;
}

//Return stored move
int ActivePokemon::getStoredMove() {
	return storedMove;
}

//Set stored damage
void ActivePokemon::setStoredDamage(int dam) {
	storedDamage = dam;
}

//Get stored damage
int ActivePokemon::getStoredDamage() {
	return storedDamage;
}

//Set disabled move
//start disable timer
void ActivePokemon::setDisable(int move) {
	disabledMove = move;
	disableTimer = 5;
}

//Return disabled move
int ActivePokemon::getDisableMove() {
	return disabledMove;
}

//Decrement disabled timer
//Returns true if still disabled
	//return false if cured or not disabled
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

//Take move name
//Return location of that move
	//Returns -1 if it doesn't exist
int ActivePokemon::findMove(string name) {
	for (int i = 0; i < moves.size(); i++) {
		if (moves[i].getIntName() == name) {
			return i;
		}
	}

	return -1;
}

//Return vector of moves
vector<ActiveMove> ActivePokemon::getMoves() {
	return moves;
}

//Debug print
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

//Print used when pokemon is active
	//Provides most information
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

//Print used for pokemon still in party
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

//Print enemy active pokemon information
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

//print enemy team information
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