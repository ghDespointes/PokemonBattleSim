#include "Move.h"

//Dummy Move, just in case
Move::Move(){
	ID = -1;
	name = "NULL";
	internalName = "NULL";
	BP = 0;
	PP = 0;
	acc = 0;

	type = Util::NONE;

	function = 0;
	effectChance = 0;
	priority = 0;
	description = "DUMMY";
}

//Actual Move cronstructor
	//Reads in a split up line from the Moves.txt file
Move::Move(vector<string> input) {
	//Convert anything to an int if its needed as a number
	ID = stoi(input[Util::MOVE_ID]);
	name = input[Util::MOVE_NAME];
	internalName = input[Util::MOVE_INT_NAME];

	function = stoi(input[Util::MOVE_FUNCTION]);
	BP = stoi(input[Util::MOVE_BP]);

	type = Util::getPokeTypeEnumFromString(input[Util::MOVE_POKE_TYPE]);
	damType = Util::getMoveTypeEnumFromString(input[Util::MOVE_MOVE_TYPE]);

	acc = stoi(input[Util::MOVE_ACC]);
	PP = stoi(input[Util::MOVE_PP]);

	effectChance = stoi(input[Util::MOVE_EFF_CHANCE]);
	priority = stoi(input[Util::MOVE_PRIORITY]);
	description = input[Util::MOVE_DESCRIPTION];
}


Move::~Move(){
}

//All the getters for the move's data
int Move::getID() {
	return ID;
}

string Move::getName() {
	return name;
}

string Move::getInternalName() {
	return internalName;
}

int Move::getFunction() {
	return function;
}

int Move::getBP() {
	return BP;
}

Util::PokeType Move::getType() {
	return type;
}

Util::MoveType Move::getMoveType() {
	return damType;
}

int Move::getAcc() {
	return acc;
}

int Move::getPP() {
	return PP;
}

int Move::getEffChance() {
	return effectChance;
}

int Move::getPriority() {
	return priority;
}

string Move::getDescription() {
	return description;
}

//print used to debug any issues
void Move::print() {
	cout << "Name: " << name << endl;
	cout << "Internal Name: " << internalName << endl;
	cout << "ID: " << ID << endl;

	cout << "Function Code: " << function << endl;
	cout << "BP: " << BP << endl;

	cout << "Type: " << Util::getPokeTypeStringFromEnum(type) << endl;
	cout << "Damage Type: " << Util::getMoveTypeStringFromEnum(damType) << endl;

	cout << "Accuracy: " << acc << endl;
	cout << "PP: " << PP << endl;
	cout << "Effect Chance: " << effectChance << endl;
	cout << "Priority: " << priority << endl;
	cout << "Description: " << description << endl;
}

//print used to show player information
void Move::printBattleInfo() {
	cout << "Name: " << name << endl;
	cout << "BP: " << BP << endl;

	cout << "Type: " << Util::getPokeTypeStringFromEnum(type) << endl;
	cout << "Damage Type: " << Util::getMoveTypeStringFromEnum(damType) << endl;

	cout << "Accuracy: " << acc << endl;
	cout << "PP: " << PP << endl;
	cout << "Priority: " << priority << endl;
	cout << "Description: " << description << endl;
}