#include "Pokemon.h"

//Dummy Pokemon
Pokemon::Pokemon() {
	ID = -1;
	name = "NULL";
	internalName = "NULL";

	stats = {0,0,0,0,0,0};

	types = { Util::NONE, Util::NONE};

	moves = {};
}

Pokemon::Pokemon(vector<string> input){
	//Parse the line and make it happen

	ID = stoi(input[Util::POKE_ID]);
	name = input[Util::POKE_NAME];
	internalName = input[Util::POKE_INT_NAME];

	stats = {stoi(input[Util::POKE_HP]), 
			stoi(input[Util::POKE_ATT]), 
			stoi(input[Util::POKE_DEF]),
			stoi(input[Util::POKE_SP_ATT]), 
			stoi(input[Util::POKE_SP_DEF]), 
			stoi(input[Util::POKE_SPEED])};

	types = {Util::getPokeTypeEnumFromString(input[Util::POKE_TYPE1]), Util::getPokeTypeEnumFromString(input[Util::POKE_TYPE2])};

	for (int i = Util::POKE_MOVE_START; i < input.size(); i++) {
		moves.push_back(input[i]);
	}
}


Pokemon::~Pokemon(){

}

string Pokemon::getName() {
	return name;
}

string Pokemon::getInternalName() {
	return internalName;
}

int Pokemon::getID() {
	return ID;
}

int Pokemon::getStat(Util::StatType stat) {
	return stats[stat];
}

vector<Util::PokeType> Pokemon::getType() {
	return types;
}

//Determine what i want to get from this
vector<string> Pokemon::getMoves() {
	return moves;
}

void Pokemon::print() {
	cout << "Name: " << name << endl;
	cout << "Internal Name: " << internalName << endl;
	cout << "ID: " << ID << endl;

	cout << "Type 1: " << Util::getPokeTypeStringFromEnum(types[0]) << endl;
	cout << "Type 2: " << Util::getPokeTypeStringFromEnum(types[1]) << endl;

	cout << "HP:\t" << getStat(Util::HP) << endl;
	cout << "Att:\t" << getStat(Util::Att) << endl;
	cout << "Def:\t" << getStat(Util::Def) << endl;
	cout << "Sp Att:\t" << getStat(Util::SpAtt) << endl;
	cout << "Sp Def:\t" << getStat(Util::SpDef) << endl;
	cout << "Speed:\t" << getStat(Util::Speed) << endl;

	cout << endl << "Possible Moves";

	for (int i = 0; i < moves.size(); i++) {
		if (i % 4 == 0) {
			cout << endl;
		}
		cout << moves[i] << "\t";
	}

	cout << endl << endl;
}
