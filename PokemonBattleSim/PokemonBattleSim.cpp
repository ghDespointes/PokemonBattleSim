// PokemonBattleSim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <ctime>

#include "Util.h"
#include "Pokemon.h"
#include "Move.h"
#include "ActivePokemon.h"
#include "FileReader.h"
#include "TurnAction.h"
#include "ActionManager.h"

using namespace std;

int main(){
	//Some Debugging cases
	/*
	string testMove = "FLAMETHROWER";
	string testPoke = "BLASTOISE";
	
	Util::getMove(testMove).print();
	cout << endl;
	Util::getPoke(testPoke).print();
	*/

	//Set up player and enemy teams
		//Parse the team files

	Team plrTeam = FileReader::getTeamInfo("PlayerTeam.txt");
	Team enemyTeam = FileReader::getTeamInfo("EnemyTeam.txt");


	ActionManager actionMan(&plrTeam, &enemyTeam);

	cout << "TEST" << endl;

	bool printInfo = true;

	srand(time(0));

	while (true) {
		string line;
		vector<string> inputs;
		TurnAction plrAction;
		TurnAction enmAction;

		//print out all needed information
		if (printInfo) {
			enemyTeam.printEnemyInfo();
			plrTeam.printBattleInfo();
		}

		//Take inputs
		plrAction = actionMan.getPlayerInput();

		if (plrAction.getParam() == "NONE") {
			printInfo = false;
			continue;
		} else if (plrAction.getParam() == "QUIT") {
			break;
		}

		printInfo = true;

		//Create AI module and have ai decide
		enmAction = TurnAction(Util::Enemy_1, Util::Attack, "1");

		//Determine turn order using a queue
		actionMan.createActionQueue(plrAction, enmAction);

		//Pop the queue to make things happen
		actionMan.performActions();

		//Check if battle is over

		cout << endl;
		system("pause");
	}

	//Determine winner and print it out

	//Wait to continue then exit

    return 0;
}

