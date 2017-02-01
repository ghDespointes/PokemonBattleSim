#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "Util.h"
#include "Pokemon.h"
#include "Move.h"
#include "Team.h"

using namespace std;

class FileReader
{
public:
	static vector<string> split(string line, char delimiter);
	static string trim(string trim);
	static string formatName(string trim);

	//static vector<vector<string>> getPokeInfo();

	static unordered_map<string,Pokemon> getPokeInfo();
	static unordered_map<string, Move> getMoveInfo();
	static Team getTeamInfo(string loc);

	FileReader();
	~FileReader();
};

