#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

class FileReader;
class Move;
class Pokemon;

using namespace std;

class Util{
public:

	//Simple enum to keep track of all the stats pokemon can have
	enum StatType {
		HP = 0,
		Att = 1,
		Def = 2,
		SpAtt = 3,
		SpDef = 4,
		Speed = 5
	};

	//Simple enum to keep track of any stat modifiers pokemon can acquire
	enum StatBoost {
		AttBoost = 0,
		DefBoost = 1,
		SpAttBoost = 2,
		SpDefBoost = 3,
		SpeedBoost = 4,
		CritBoost = 5,
		AccBoost = 6,
		EvasionBoost = 7,
	};

	//Enum describing each pokemon type and a 19th NULL type for a typeless interactions
	enum PokeType {
		Normal = 0,
		Fighting = 1,
		Flying = 2,
		Poison = 3,
		Ground = 4,
		Rock = 5,
		Bug = 6,
		Ghost = 7,
		Steel = 8,
		Fire = 9,
		Water = 10,
		Grass = 11,
		Electric = 12,
		Psychic = 13,
		Ice = 14,
		Dragon = 15,
		Dark = 16,
		Fairy = 17,
		NONE = 18
	};

	//Enum describing the 3 types of moves
	enum MoveType {
		Physical,
		Special,
		Status
	};

	//Enum describing the 2 main actions and Other used as a catch all
		//for any bad actions
	enum Action {
		Switch,
		Attack,
		Other
	};

	//Enum describing the users and locations of pokemon
		//Player_2 and Enemy_2 can be ignored
		//In the future it can be implmented for 2v2 battles
		//but those aren't currently implemented
	enum User {
		Player_1 = 0,
		Enemy_1 = 1,
		Player_2 = 2,
		Enemy_2 = 3
	};

	//Enum describing any permanent status condition that persists
	enum StatusCond {
		Standard,
		Fainted,
		Asleep,
		Frozen,
		Burned,
		Paralyzed,
		Poisoned,
		Badly_Poisoned
	};

	//Enum describing any volatile status condition that is lost on switch
	enum VolStatus {
		Confused = 0,
		Bound = 1,
		Cant_Escape = 2,
		Flinched = 3,
		Skip = 4
	};

	//All the stored information about pokemon and moves
	static const unordered_map<string, Move> moveHash;
	static const unordered_map<string, Pokemon> pokeHash;

	//Configuration files for pokemon
	static const string POKE_DEF_LOC;
	static const string MOVE_DEF_LOC;

	static const string PLAYER_TEAM;
	static const string ENEMY_TEAM;

	//Pokemon Data locations
	static const int POKE_ID = 0;
	static const int POKE_NAME = 1;
	static const int POKE_INT_NAME = 2;
	static const int POKE_TYPE1 = 3;
	static const int POKE_TYPE2 = 4;

	static const int POKE_HP = 5;
	static const int POKE_ATT = 6;
	static const int POKE_DEF = 7;
	static const int POKE_SP_ATT = 8;
	static const int POKE_SP_DEF = 9;
	static const int POKE_SPEED = 10;

	static const int POKE_MOVE_START = 11;

	//Move data locations
	static const int MOVE_ID = 0;
	static const int MOVE_NAME = 1;
	static const int MOVE_INT_NAME = 2;

	static const int MOVE_FUNCTION = 3;
	static const int MOVE_BP = 4;
	static const int MOVE_POKE_TYPE = 5;
	static const int MOVE_MOVE_TYPE = 6;

	static const int MOVE_ACC = 7;
	static const int MOVE_PP = 8;
	static const int MOVE_EFF_CHANCE = 9;
	static const int MOVE_PRIORITY = 10;
	static const int MOVE_DESCRIPTION = 11;

	//Active pokemon data locations
	static const int ACTIVE_MON_NAME = 0;
	static const int ACTIVE_MON_INT_NAME = 1;
	static const int ACTIVE_MON_ITEM = 2;
	static const int ACTIVE_MON_LEVEL = 3;
	static const int ACTIVE_MON_ABILITY = 4;

	static const int ACTIVE_MON_EV_HP = 5;
	static const int ACTIVE_MON_EV_ATT = 6;
	static const int ACTIVE_MON_EV_DEF = 7;
	static const int ACTIVE_MON_EV_SP_ATT = 8;
	static const int ACTIVE_MON_EV_SP_DEF = 9;
	static const int ACTIVE_MON_EV_SPEED = 10;

	static const int ACTIVE_MON_NATURE = 11;

	static const int ACTIVE_MON_IV_HP = 12;
	static const int ACTIVE_MON_IV_ATT = 13;
	static const int ACTIVE_MON_IV_DEF = 14;
	static const int ACTIVE_MON_IV_SP_ATT = 15;
	static const int ACTIVE_MON_IV_SP_DEF = 16;
	static const int ACTIVE_MON_IV_SPEED = 17;

	static const int ACTIVE_MON_MOVE_LOC = 18;

	//Natures affect on stats
	static const double NATURE_STANDARD;
	static const double NATURE_INCREASE;
	static const double NATURE_DECREASE;

	//MAX EV Values
	static const int EV_MAX_TOTAL = 510;
	static const int EV_MAX = 252;

	//MAX IV Value
	static const int IV_MAX = 31;

	//AI Status Move States
	static const int AI_HIGH_HEALTH = 70;
	static const int AI_MED_HEALTH = 30;

	static const int AI_HIGH_CHANCE = 50;
	static const int AI_MED_CHANCE = 35;
	static const int AI_LOW_CHANCE = 15;

	//Paralyze Speed Affectiveness 1/N
	static const int PARA_SPEED_EFFECT = 2;
	static const int PARA_CHANCE = 4;

	//Unfreeze chance 1/N
	static const int UNFREEZE_CHANCE = 5;

	//Burn Damage
	static const double BURN_DAMAGE;
	static const int BURN_ATT_REDUCTION = 2;

	//Poison Damage
	static const double POISON_DAMAGE;

	//Bad Poison Damage
	static const double BAD_POISON_DAMAGE;

	//Confusion hit chance [0,99]
	static const int CONFUSION_CHANCE = 33;

	//Minimum Accuracy Hit Chance
	static const int MIN_HIT_CHANCE = 33;

	//Crit chance by level
	static const double CRIT_LEVEL_0;
	static const double CRIT_LEVEL_1;
	static const double CRIT_LEVEL_2;
	static const double CRIT_LEVEL_3;

	//Stat Modifiers by level
	static const double STAT_MOD_LEVEL_N6;
	static const double STAT_MOD_LEVEL_N5;
	static const double STAT_MOD_LEVEL_N4;
	static const double STAT_MOD_LEVEL_N3;
	static const double STAT_MOD_LEVEL_N2;
	static const double STAT_MOD_LEVEL_N1;
	static const double STAT_MOD_LEVEL_0;
	static const double STAT_MOD_LEVEL_1;
	static const double STAT_MOD_LEVEL_2;
	static const double STAT_MOD_LEVEL_3;
	static const double STAT_MOD_LEVEL_4;
	static const double STAT_MOD_LEVEL_5;
	static const double STAT_MOD_LEVEL_6;

	//Accuracies by level
	//Stat Modifiers by level
	static const double ACC_MOD_LEVEL_N6;
	static const double ACC_MOD_LEVEL_N5;
	static const double ACC_MOD_LEVEL_N4;
	static const double ACC_MOD_LEVEL_N3;
	static const double ACC_MOD_LEVEL_N2;
	static const double ACC_MOD_LEVEL_N1;
	static const double ACC_MOD_LEVEL_0;
	static const double ACC_MOD_LEVEL_1;
	static const double ACC_MOD_LEVEL_2;
	static const double ACC_MOD_LEVEL_3;
	static const double ACC_MOD_LEVEL_4;
	static const double ACC_MOD_LEVEL_5;
	static const double ACC_MOD_LEVEL_6;

	//Min Max StatBoosts
	static const int MAX_STAT_BOOST = 6;
	static const int MIN_STAT_BOOST = -6;

	//Priority Level Min Max
	static const int MAX_PRIORITY = 6;
	static const int MIN_PRIORITY = -6;

	//Stab Effectiveness
	static const double STAB_BOOST;

	//Crit multiplier
	static const double CRIT_BOOST;

	//Multihit move chances out of 100
	static const int HIT2 = 333;
	static const int HIT3 = 333;
	static const int HIT4 = 167;

	//ONE HIT KO Level Offset
	static const int OHKO_OFFSET = 30;

	//Returns information about moves and pokemon
	static Move getMove(string intName);
	static Pokemon getPoke(string intName);

	//Checks to see if a pokemon exists
	static int checkMove(string intName);
	static int checkPoke(string intName);

	//Convert between enums and string
		//Used when parsing and printing information to the player
	static string getPokeTypeStringFromEnum(PokeType type);
	static PokeType getPokeTypeEnumFromString(string type);

	static string getMoveTypeStringFromEnum(MoveType type);
	static MoveType getMoveTypeEnumFromString(string type);

	static vector<double> getNatureFromString(string nature);

	static string getStatusStringFromEnum(Util::StatusCond status);

	static string getStatBoostStringFromEnum(Util::StatBoost stat);
};

