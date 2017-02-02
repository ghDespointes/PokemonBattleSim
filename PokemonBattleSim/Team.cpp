#include "Team.h"

Team::Team() {
}

//Sets the team
Team::Team(vector<ActivePokemon> inputteam) {
	team = inputteam;
}

Team::~Team() {
}

//Take int
//Swaps the active pokemon for the one at location int
void Team::makeActive(int poke) {
	ActivePokemon temp = team[poke];
	team[poke] = team[0];
	team[0] = temp;
}

//Returns pointer of active pokemon
ActivePokemon* Team::getActive() {
	return &team[0];
}

//Return full team
vector<ActivePokemon> Team::getTeam() {
	return team;
}

//Take name
//Return location of that pokemon
	//Return -1 is it doesn't exists
int Team::findPoke(string name) {
	for (int i = 0; i < team.size(); i++) {
		if (team[i].getIntName() == name) {
			return i;
		}
	}

	return -1;
}

//Print all the debug information of each pokemon
void Team::print() {
	cout << "Printing Team" << endl;

	for (int i = 0; i < team.size(); i++) {
		team[i].print();

		cout << endl;
	}
}

//Print the most information about the active pokemon and useful information about the others
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

//Print information the player needs to know about the enemy team
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
