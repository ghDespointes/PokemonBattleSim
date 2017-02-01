#include "ActiveMove.h"



ActiveMove::ActiveMove() {
	name = "NONE";
	internalName = "NONE";
	ID = -1;

	maxPP = 0;
	currPP = maxPP;
}

ActiveMove::ActiveMove(string name) {
	internalName = name;
	
	Move move = Util::getMove(name);

	this->name = move.getName();

	ID = move.getID();

	maxPP = 1.6*move.getPP();
	currPP = maxPP;

}

ActiveMove::~ActiveMove() {
}

int ActiveMove::useMove() {


	return 0;
}

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

void ActiveMove::setPP(int pp) {
	currPP = pp;
}

int ActiveMove::getPP() {
	return currPP;
}

void ActiveMove::print() {
	cout << "Name: " << name << endl;
	cout << "Internal Name: " << internalName << endl;
	cout << "ID: " << ID << endl;

	Util::getMove(internalName).print();

	cout << "MaxPP: " << maxPP << endl;
	cout << "Current PP: " << currPP << endl;

	cout << endl;
}

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