#include "Team.h"

Team::Team() {
}

Team::Team(vector<ActivePokemon> inputteam) {
	team = inputteam;
}

Team::~Team() {
}

void Team::makeActive(int poke) {
	ActivePokemon temp = team[poke];
	team[poke] = team[0];
	team[0] = temp;
}

ActivePokemon* Team::getActive() {
	return &team[0];
}

vector<ActivePokemon> Team::getTeam() {
	return team;
}

int Team::findPoke(string name) {
	for (int i = 0; i < team.size(); i++) {
		if (team[i].getIntName() == name) {
			return i;
		}
	}

	return -1;
}

void Team::print() {
	cout << "Printing Team" << endl;

	for (int i = 0; i < team.size(); i++) {
		team[i].print();

		cout << endl;
	}
}

void Team::printBattleInfo() {
	cout << "PLAYER INFORMATION:" << endl;

	team[0].printBattleInfo();

	cout << "Switch Pokemon:" << endl;
	for (int i = 1; i < team.size(); i++) {
		cout << "[" << i << "]" << endl;
		team[i].printBasicInfo();
		cout << endl;
	}
}

void Team::printEnemyInfo() {
	cout << "ENEMY INFORMATION:" << endl;
	team[0].printEnemyInfo();

	cout << endl;

	cout << "Team:" << endl;

	for (int i = 1; i < team.size(); i++) {
		team[i].printEnemyBasicInfo();
		cout << endl;
	}
}
