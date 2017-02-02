// PokemonBattleSim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <ctime>

#include "Team.h"
#include "FileReader.h"
#include "ActionManager.h"
#include "TurnAction.h"
#include "AIManager.h"

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
	AIManager aiMan(&enemyTeam, &plrTeam);
	ActionManager actionMan(&plrTeam, &enemyTeam, &aiMan);

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
		enmAction = aiMan.determineAction(Util::Attack);
			
		//TurnAction(Util::Enemy_1, Util::Attack, "1");

		//Determine turn order and populate teh order into a queue
		actionMan.createActionQueue(plrAction, enmAction);

		//Pop the queue to perform each of the actions in the right order
		actionMan.performActions();

		//Check if battle is over
		//Determine winner and print it out
		if (actionMan.checkTeamLoss()) {
			break;
		}

		//Wait for player input to ensure they see what occured that turn
		system("pause");
	}

	//Wait for player input to ensure they see what occured that turn
		system("pause");

    return 0;
}

