#include "ActiveMove.h"

//Dummy active move
ActiveMove::ActiveMove() {
	name = "NONE";
	internalName = "NONE";
	ID = -1;

	maxPP = 0;
	currPP = maxPP;
}

//Real constructor
ActiveMove::ActiveMove(string name) {
	internalName = name;
	
	Move move = Util::getMove(name);

	this->name = move.getName();

	ID = move.getID();

	//Max PP is always 1.6* times more than what the game specifies
	maxPP = 1.6*move.getPP();
	currPP = maxPP;

}

ActiveMove::~ActiveMove() {
}

//Some getters
string ActiveMove::getName() {
	return name;
}

string ActiveMove::getIntName() {
	return internalName;
}

int ActiveMove::getID() {
	return ID;
}

int ActiveMove::getMaxPP() {
	return maxPP;
}

//Getters and setter for PP
void ActiveMove::setPP(int pp) {
	currPP = pp;
}

int ActiveMove::getPP() {
	return currPP;
}

//Debug printing everything
void ActiveMove::print() {
	cout << "Name: " << name << endl;
	cout << "Internal Name: " << internalName << endl;
	cout << "ID: " << ID << endl;

	Util::getMove(internalName).print();

	cout << "MaxPP: " << maxPP << endl;
	cout << "Current PP: " << currPP << endl;

	cout << endl;
}

//Print to display information for the player
void ActiveMove::printBattleInfo() {
	Move move = Util::getMove(internalName);

	cout << name << endl;
	cout << Util::getPokeTypeStringFromEnum(move.getType()) << " | ";
	cout << Util::getMoveTypeStringFromEnum(move.getMoveType()) << endl;

	if (move.getMoveType() != Util::Status) {
		cout << "BP: " << move.getBP() << endl;
	}
	cout << "PP: " << currPP << "/" << maxPP << endl; 
	cout << "Accuracy: " << move.getAcc() << endl;

	cout << endl;
}