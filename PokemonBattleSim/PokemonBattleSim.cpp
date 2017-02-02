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

	//Initialize ActionManager
		//Will be in charge of performing any actions the player needs
	ActionManager actionMan(&plrTeam, &enemyTeam);

	//Controls whether game status info will be printed for the player
	bool printInfo = true;

	//Set seed as current time for any randomness we will need
	srand(time(0));

	//Main game loop 
	while (true) {
		string line;
		vector<string> inputs;
		TurnAction plrAction;
		TurnAction enmAction;

		//Print out all needed information
		if (printInfo) {
			enemyTeam.printEnemyInfo();
			plrTeam.printBattleInfo();
		}

		//Accept player inputs
		plrAction = actionMan.getPlayerInput();

		//If the player gives a bad input getPlayerInput will return a null action
			//and the loop will start over
		if (plrAction.getParam() == "NONE") {
			printInfo = false;
			continue;

		//If the player wants to quit leave the main loop
		} else if (plrAction.getParam() == "QUIT") {
			break;
		}

		printInfo = true;

		//Create AI module and have ai decide
		//Call on AI module to choose most appropriate action
		enmAction = TurnAction(Util::Enemy_1, Util::Attack, "1");

		//Determine turn order and populate teh order into a queue
		actionMan.createActionQueue(plrAction, enmAction);

		//Pop the queue to perform each of the actions in the right order
		actionMan.performActions();

		//Check if battle is over

		//Wait for player input to ensure they see what occured that turn
		cout << endl;
		system("pause");
	}

	//Determine winner and print it out

	//Wait to continue then exit

    return 0;
}

