#include "FileReader.h"

FileReader::FileReader()
{
}


FileReader::~FileReader()
{
}

vector<string> FileReader::split(string line, char delimiter) {
	stringstream ss(line);
	string token;
	vector<string> splitString;

	while (getline(ss, token, delimiter)) {
		if (token.find('"') <= token.size()) {
			string temp;

			while (getline(ss, temp, delimiter)) {
				//To Do: Change to Delimiter
				token = token + delimiter +temp;
			}
		}

		splitString.push_back(token);
	}

	return splitString;
}

string FileReader::trim(string line){
	int frontSpace = line.find_first_not_of(" ");
	int endSpace = line.find_last_not_of(" ") + 1;

	return line.substr(frontSpace, endSpace - frontSpace);
}

string FileReader::formatName(string line) {
	transform(line.begin(), line.end(), line.begin(), ::toupper);
	line.erase(remove(line.begin(), line.end(), ' '), line.end());
	
	return line;
}

unordered_map<string, Move> FileReader::getMoveInfo() {
	ifstream moveFile(Util::MOVE_DEF_LOC);

	string line;

	unordered_map<string, Move> moveHash;

	while (getline(moveFile, line)) {
		if (line[0] != '#') {
			vector<string> moveInfo = split(line, ',');

			/*for (int i = 0; i < moveInfo.size(); i++) {
			cout << moveInfo[i] << "+" << endl;
			}

			cout << endl;*/

			Move newMove(moveInfo);
			moveHash.emplace(moveInfo[Util::MOVE_INT_NAME], newMove);
		}
	}


	cout << "Finished scanning Moves" << endl;
	return moveHash;
}

unordered_map<string, Pokemon> FileReader::getPokeInfo() {
	ifstream pokeFile(Util::POKE_DEF_LOC);

	string line;

	unordered_map<string, Pokemon> pokeHash;

	int i = 0;

	while (getline(pokeFile, line)) {
		if (line[0] == '[') {
			vector<string> pokeInfo;

			line = trim(line);

			pokeInfo.push_back(line.substr(1, line.find("]",1) - 1));

			while (getline(pokeFile, line)) {
				if (line == "") { break; }

				line = trim(line);

				if (line.substr(0, 5) == "Name=") {
					line = line.substr(5);

					pokeInfo.push_back(line);
					pokeInfo.push_back(formatName(line));

				} else if (line.substr(0, 5) == "Type=") {
					line = line.substr(5);

					vector<string> types = split(line, ',');

					pokeInfo.push_back(types[0]);

					if (types.size() == 1) {
						pokeInfo.push_back("");

					} else {
						pokeInfo.push_back(types[1]);
					}

				}
				else if (line.substr(0, 10) == "BaseStats=") {
					line = line.substr(10);

					vector<string> stats = split(line, ',');

					for (int i = 0; i < stats.size(); i++) {
						pokeInfo.push_back(stats[i]);
					}

				}
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

			Pokemon newPoke(pokeInfo);

			pokeHash.emplace(pokeInfo[Util::POKE_INT_NAME], newPoke);
		}
	}

	cout << "Finished scanning Pokemon" << endl;
	return pokeHash;
}

Team FileReader::getTeamInfo(string loc) {
	ifstream teamFile(loc);

	string line;

	vector<ActivePokemon> activePokes;

	while (getline(teamFile, line)) {
		if (line[0] != '#' && line != "") {
			vector<string> activeMonInfo;
			
			vector<string> data = split(line, '@');

			data[0] = trim(data[0]);
			data[1] = trim(data[1]);


			//Format
			//Name, IntName, Item, Level, AbilityName, EVs {}, Nature, IVs {}, Moves{}
			activeMonInfo.push_back(data[0]);
			activeMonInfo.push_back(formatName(data[0]));
			activeMonInfo.push_back(data[1]);

			activeMonInfo.push_back("100");

			string abilityName;
			vector<string> evCleaned = { "0","0","0","0","0","0" };
			string nature;
			vector<string> ivCleaned = { "31","31","31","31","31","31" };
			vector<string> moves;

			//Clean this up so that it doen't break when the line is too short
			while (getline(teamFile, line)) {
				if (line == "") { break; }

				line = trim(line);

				//cout << line << endl;

				int lineSize = line.size();

				if (line.substr(0, 8) == "Ability:") {
					if (lineSize <= 8) {
						cout << "No ability was entered, so we'll use a random one" << endl;
						continue;
					}
					
					line = line.substr(8);
					line = trim(line);

					abilityName = formatName(line);

				} else if (line.substr(0, 4) == "EVs:") {
					if (lineSize <= 4){
						cout << "No EVs were stated so they're set to 0" << endl;
						continue;
					}

					line = line.substr(4);
					line = trim(line);

					vector<string> evs = split(line, '/');

					for (int i = 0; i < evs.size(); i++) {
						evs[i] = trim(evs[i]);

						int loc;

						if ((loc = evs[i].find("Hp")) <= evs[i].size()) {
							evCleaned[0] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("Atk")) <= evs[i].size()) {
							evCleaned[1] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("Def")) <= evs[i].size()) {
							evCleaned[2] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("SpA")) <= evs[i].size()) {
							evCleaned[3] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("SpD")) <= evs[i].size()) {
							evCleaned[4] = trim(evs[i].substr(0, loc));

						} else if ((loc = evs[i].find("Spe")) <= evs[i].size()) {
							evCleaned[5] = trim(evs[i].substr(0, loc));
						}
					}
					
				} else if (line.substr(lineSize >= 6 ? lineSize - 6 : 0) == "Nature") {
					line = line.substr(0, line.find("Nature"));
					line = trim(line);

					nature = line;

				} else if (line.substr(0, 4) == "IVs:") {
					if (lineSize <= 4) {
						cout << "No Ivs were states so they were set to 31" << endl;
						continue;
					}

					line = line.substr(4);
					line = trim(line);

					vector<string> ivs = split(line, '/');

					for (int i = 0; i < ivs.size(); i++) {
						ivs[i] = trim(ivs[i]);

						int loc;

						if ((loc = ivs[i].find("Hp")) <= ivs[i].size()) {
							ivCleaned[0] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("Atk")) <= ivs[i].size()) {
							ivCleaned[1] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("Def")) <= ivs[i].size()) {
							ivCleaned[2] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("SpA")) <= ivs[i].size()) {
							ivCleaned[3] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("SpD")) <= ivs[i].size()) {
							ivCleaned[4] = trim(ivs[i].substr(0, loc));

						}
						else if ((loc = ivs[i].find("Spe")) <= ivs[i].size()) {
							ivCleaned[5] = trim(ivs[i].substr(0, loc));
						}
					}

				} else if (line[0] == '-') {
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

			//Add Moves
			for (int i = 0; i < moves.size(); i++) {
				activeMonInfo.push_back(moves[i]);
			}

			//Debug Printing everything in order
			/*for (int i = 0; i < activeMonInfo.size(); i++) {
				cout << activeMonInfo[i] << "+" << endl;
			}*/

			ActivePokemon newPoke(activeMonInfo);
			activePokes.push_back(newPoke);
		}
	}

	cout << "Finished scanning Team" << endl;

	Team team(activePokes);

	return team;
}