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

//Very important utility class
//Primary function is to read the important files and do any sting functions
class FileReader
{
public:
	//Split the strings by some sort of delimieter
	static vector<string> split(string line, char delimiter);

	//Trim the strings by getting rid of spaces at the front and end
	static string trim(string trim);

	//Formats any name into a form that the code can read
		//In this case its all caps without spaces
	static string formatName(string trim);

	//Reads the Pokemon file, creates, and populates the map that stores all
		//the pokemon data for Util.h
	static unordered_map<string,Pokemon> getPokeInfo();

	//Reads the Move file, creates, and populates the map that stores all
		//the move data for Util.h
	static unordered_map<string, Move> getMoveInfo();

	//Reads teh team files and creates the team 
	static Team getTeamInfo(string loc);
};

