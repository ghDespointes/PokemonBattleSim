#include "FileReader.h"

//Takes a string and delimiter
//Returns a vector<string> of each piece leaving things in quotes as a single piece
vector<string> FileReader::split(string line, char delimiter) {
	stringstream ss(line);
	string token;
	vector<string> splitString;

	//Parses through and grabs pieces of the string based on delimieter
	while (getline(ss, token, delimiter)) {

		//If a " is encountered put the pieces back together to leave it as a whole line
		if (token.find('"') <= token.size()) {
			string temp;

			while (getline(ss, temp, delimiter)) {
				token = token + delimiter +temp;
			}
		}

		splitString.push_back(token);
	}

	return splitString;
}

//Takes a string
//Returns the string without spaces in front and behind
string FileReader::trim(string line){
	int frontSpace;
	int endSpace;

	frontSpace = line.find_first_not_of(" ");
	endSpace = line.find_last_not_of(" ");

	//If its all spaces just return an empty string
	if (endSpace == string::npos) {
		return "";
	}

	return line.substr(frontSpace, endSpace + 1 - frontSpace);
}

//Takes a string
//Returns the string in all caps and without spaces
string FileReader::formatName(string line) {
	transform(line.begin(), line.end(), line.begin(), ::toupper);
	line.erase(remove(line.begin(), line.end(), ' '), line.end());
	
	return line;
}

//Parses Move file and returns a map of all the moves read
unordered_map<string, Move> FileReader::getMoveInfo() {
	ifstream moveFile(Util::MOVE_DEF_LOC);

	string line;

	unordered_map<string, Move> moveHash;

	//Pass through each line of moves
	while (getline(moveFile, line)) {
		//Ignore any line starting in #
		if (line[0] != '#') {
			vector<string> moveInfo = split(line, ',');

			/*for (int i = 0; i < moveInfo.size(); i++) {
			cout << moveInfo[i] << "+" << endl;
			}

			cout << endl;*/

			//Split the line and create a move from it
				//Add the newMove to the final map
			Move newMove(moveInfo);
			moveHash.emplace(moveInfo[Util::MOVE_INT_NAME], newMove);
		}
	}


	//cout << "Finished scanning Moves" << endl;
	return moveHash;
}

//Parses Pokemon file and returns a map of all the Pokemon read
unordered_map<string, Pokemon> FileReader::getPokeInfo() {
	ifstream pokeFile(Util::POKE_DEF_LOC);

	string line;

	unordered_map<string, Pokemon> pokeHash;

	int i = 0;

	//Read each line until you find the start of a pokemon, a [#]
	while (getline(pokeFile, line)) {
		if (line[0] == '[') {
			vector<string> pokeInfo;

			line = trim(line);

			//Add the ID to the Pokemon input vector
			pokeInfo.push_back(line.substr(1, line.find("]",1) - 1));

			//Keep reading lines to find out more information about the pokemon
				//Stop when a space is found
			while (getline(pokeFile, line)) {
				if (line == "") { break; }

				line = trim(line);

				//If name is found, add the name
				if (line.substr(0, 5) == "Name=") {
					line = line.substr(5);

					pokeInfo.push_back(line);
					pokeInfo.push_back(formatName(line));

				//If type is found, add each type to the vector
				} else if (line.substr(0, 5) == "Type=") {
					line = line.substr(5);

					vector<string> types = split(line, ',');

					pokeInfo.push_back(types[0]);

					//If there is no second type add an empty space to represent no type
					if (types.size() == 1) {
						pokeInfo.push_back("");

					} else {
						pokeInfo.push_back(types[1]);
					}

				}
				//If BaseStats is found, pass each stat to the vector
				else if (line.substr(0, 10) == "BaseStats=") {
					line = line.substr(10);

					vector<string> stats = split(line, ',');

					for (int i = 0; i < stats.size(); i++) {
						pokeInfo.push_back(stats[i]);
					}

				}
				//If Moves is found, read each move and add it to the final output vector
				else if (line.substr(0, 6) == "Moves=") {
					line = line.substr(6);

					vector<string> moves = split(line, ',');

					for (int i = 0; i < moves.size(); i++) {
						pokeInfo.push_back(moves[i]);
					}

				}
			}

			//Just a debugging feature to make sure things are being parsed properly
			/*for (int i = 0; i < pokeInfo.size(); i++) {
				cout << pokeInfo[i] << endl;
			}

			cout << endl;*/

			//With the final vector create a new pokemon and insert it to the map
			Pokemon newPoke(pokeInfo);

			pokeHash.emplace(pokeInfo[Util::POKE_INT_NAME], newPoke);
		}
	}

	//cout << "Finished scanning Pokemon" << endl;
	return pokeHash;
}

//Parses Team file and returns a Team for the file read
Team FileReader::getTeamInfo(string loc) {
	ifstream teamFile(loc);

	string line;

	vector<ActivePokemon> activePokes;

	//Read each line and ignore lines that are empty or start with #
	while (getline(teamFile, line)) {
		if (line[0] != '#' && line != "") {
			//Format
			//Name, IntName, Item, Level, AbilityName, EVs {}, Nature, IVs {}, Moves{}
			vector<string> activeMonInfo;
			vector<string> itemNamePair;
			string pokeName;
			string itemName;

			string formatedName;

			//If an item exists,
			//Split the first line into Name and item
			//Note Items are not implemented currently
			int itemLoc = line.find("@");

			if (itemLoc != string::npos) {
				itemNamePair = split(line, '@');

				pokeName = trim(itemNamePair[0]);

				if (itemNamePair.size() < 2) {
					itemName = "";
				}
				else {
					itemName = trim(itemNamePair[1]);
				}
			}
			else {
				pokeName = trim(line);
				itemName = "";
			}

			formatedName = formatName(pokeName);

			if (Util::checkPoke(formatedName) == 0) {
				cout << pokeName << " doesn't exist. Please make sure you typed it correctly." << endl;
				return Team();
			}

			//Add the name, internal Name, and item into the vector
			activeMonInfo.push_back(pokeName);
			activeMonInfo.push_back(formatedName);
			activeMonInfo.push_back(itemName);

			//The level default is level 100
			activeMonInfo.push_back("100");

			//Set up all the other parts of a pokemon in a team with defaults
			string abilityName;
			vector<string> evCleaned = { "0","0","0","0","0","0" };
			string nature;
			vector<string> ivCleaned = { "31","31","31","31","31","31" };
			vector<string> moves;

			//Read the rest of the pokemon and stop when a space is found
			while (getline(teamFile, line)) {
				if (line == "") { break; }

				line = trim(line);

				int lineSize = line.size();

				//Set ability if it is found
					//Note abilities are not implmented
				if (line.substr(0, 8) == "Ability:") {
					if (lineSize <= 8) {
						cout << "No ability was entered, so we'll use a random one" << endl;
						continue;
					}
					
					line = line.substr(8);
					line = trim(line);

					abilityName = formatName(line);
				} 
				//If Evs is found, parse the line to discover what value is applied for each stat
				else if (line.substr(0, 4) == "EVs:") {
					//If its left blank tell the player
					if (lineSize <= 4){
						cout << "No EVs were stated so they're set to 0" << endl;
						continue;
					}

					line = line.substr(4);
					line = trim(line);

					vector<string> evs = split(line, '/');

					//Look through and see what Evs are mentioned and what values apply to them
					for (int i = 0; i < evs.size(); i++) {
						evs[i] = trim(evs[i]);

						int loc;

						if ((loc = evs[i].find("Hp")) != string::npos) {
							evCleaned[0] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("Atk")) != string::npos) {
							evCleaned[1] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("Def")) != string::npos) {
							evCleaned[2] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("SpA")) != string::npos) {
							evCleaned[3] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("SpD")) != string::npos) {
							evCleaned[4] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("Spe")) != string::npos) {
							evCleaned[5] = trim(evs[i].substr(0, loc));
						}
					}
				} 
				//If nature is found store it
				//Its convoluted because moves may have the word nature in their names
				else if (line.substr(lineSize >= 6 ? lineSize - 6 : 0) == "Nature") {
					line = line.substr(0, line.find("Nature"));
					line = trim(line);

					nature = line;
				} 
				//If Ivs is found, parse the line to discover what value is applied for each stat
				else if (line.substr(0, 4) == "IVs:") {
					//If its left blank tell the player
					if (lineSize <= 4) {
						cout << "No Ivs were states so they were set to 31" << endl;
						continue;
					}

					line = line.substr(4);
					line = trim(line);

					vector<string> ivs = split(line, '/');

					//Look through and see what Ivs are mentioned and what values apply to them
					for (int i = 0; i < ivs.size(); i++) {
						ivs[i] = trim(ivs[i]);

						int loc;

						if ((loc = ivs[i].find("Hp")) != string::npos) {
							ivCleaned[0] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("Atk")) != string::npos) {
							ivCleaned[1] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("Def")) != string::npos) {
							ivCleaned[2] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("SpA")) != string::npos) {
							ivCleaned[3] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("SpD")) != string::npos) {
							ivCleaned[4] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("Spe")) != string::npos) {
							ivCleaned[5] = trim(ivs[i].substr(0, loc));
						}
					}
				} 
				//If a - is found, it means theres a move add it and deal with it later
				else if (line[0] == '-') {
					line = line.substr(1);
					line = trim(line);

					moves.push_back(formatName(line));
				}
			}

			//Add Ability
			activeMonInfo.push_back(abilityName);

			//Add Evs
			for (int i = 0; i < evCleaned.size(); i++) {
				activeMonInfo.push_back(evCleaned[i]);
			}

			//Add Nature
			activeMonInfo.push_back(nature);

			//Add Ivs
			for (int i = 0; i < ivCleaned.size(); i++) {
				activeMonInfo.push_back(ivCleaned[i]);
			}

			//Add All Moves even if there's more than 4
			for (int i = 0; i < moves.size(); i++) {
				activeMonInfo.push_back(moves[i]);
			}

			//Debug Printing everything in order
			/*for (int i = 0; i < activeMonInfo.size(); i++) {
				cout << activeMonInfo[i] << "+" << endl;
			}*/

			//Create each new ActivePokemon and store them
			ActivePokemon newPoke(activeMonInfo);
			activePokes.push_back(newPoke);
		}
	}

	//cout << "Finished scanning Team" << endl;

	//Create the the teams using all the pokemon found
	Team team(activePokes);

	return team;
}