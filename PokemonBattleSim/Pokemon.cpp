#include "Pokemon.h"

//Dummy Pokemon, in case we need it
Pokemon::Pokemon() {
	ID = -1;
	name = "NULL";
	internalName = "NULL";

	stats = {0,0,0,0,0,0};

	types = { Util::NONE, Util::NONE};

	moves = {};
}

//Actual pokemon cronstructor
	//Reads in a split up line from the Pokemon.txt file
Pokemon::Pokemon(vector<string> input){
	//Convert anything to an int if its needed as a number
	ID = stoi(input[Util::POKE_ID]);
	name = input[Util::POKE_NAME];
	internalName = input[Util::POKE_INT_NAME];

	stats = {stoi(input[Util::POKE_HP]), 
			stoi(input[Util::POKE_ATT]), 
			stoi(input[Util::POKE_DEF]),
			stoi(input[Util::POKE_SP_ATT]), 
			stoi(input[Util::POKE_SP_DEF]), 
			stoi(input[Util::POKE_SPEED])};

	//Convert the string into the type Enum
	types = {Util::getPokeTypeEnumFromString(input[Util::POKE_TYPE1]), Util::getPokeTypeEnumFromString(input[Util::POKE_TYPE2])};

	//Add all possible moves that can be used by the pokemon
	for (int i = Util::POKE_MOVE_START; i < input.size(); i++) {
		moves.push_back(input[i]);
	}
}


Pokemon::~Pokemon(){

}

//All the getters for the pokemon's data
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

vector<string> Pokemon::getMoves() {
	return moves;
}

//print used to debug any issues
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
