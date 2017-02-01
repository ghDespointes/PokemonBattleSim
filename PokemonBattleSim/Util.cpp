#include "Util.h"
#include "FileReader.h"
#include "Move.h"
#include "Pokemon.h"

const string Util::POKE_DEF_LOC = "Pokemon.txt";
const string Util::MOVE_DEF_LOC = "Moves.txt";

const double Util::NATURE_STANDARD = 1.0;
const double Util::NATURE_INCREASE = 1.1;
const double Util::NATURE_DECREASE = .9;

//Sets up all the pokemon and move Data that we can refer to
const unordered_map<string, Move> Util::moveHash = FileReader::getMoveInfo();
const unordered_map<string, Pokemon> Util::pokeHash = FileReader::getPokeInfo();

Move Util::getMove(string intName) {
	return moveHash.at(intName);
}

Pokemon Util::getPoke(string intName) {
	return pokeHash.at(intName);
}

string Util::getPokeTypeStringFromEnum(PokeType type) {
	switch (type) {
	case Util::Normal: return "Normal";
	case Util::Fire: return "Fire";
	case Util::Water: return "Water";
	case Util::Electric: return "Electric";
	case Util::Grass: return "Grass";
	case Util::Ice: return "Ice";
	case Util::Fighting: return "Fighting";
	case Util::Poison: return "Poison";
	case Util::Ground: return "Ground";
	case Util::Flying: return "Flying";
	case Util::Psychic: return "Psychic";
	case Util::Bug: return "Bug";
	case Util::Rock: return "Rock";
	case Util::Ghost: return "Ghost";
	case Util::Dragon: return "Dragon";
	case Util::Dark: return "Dark";
	case Util::Steel: return "Steel";
	case Util::Fairy: return "Fairy";
	case Util::NONE: return "NONE";

	default: return "That type doesn't exist";
	}
}

Util::PokeType Util::getPokeTypeEnumFromString(string type) {
	if (type == "Normal") {
		return Util::Normal;
	} else if (type == "Fire") {
		return Util::Fire;
	} else if (type == "Water") {
		return Util::Water;
	} else if (type == "Electric") {
		return Util::Electric;
	} else if (type == "Grass") {
		return Util::Grass;
	} else if (type == "Ice") {
		return Util::Ice;
	} else if (type == "Fighting") {
		return Util::Fighting;
	} else if (type == "Poison") {
		return Util::Poison;
	} else if (type == "Ground") {
		return Util::Ground;
	} else if (type == "Flying") {
		return Util::Flying;
	} else if (type == "Psychic") {
		return Util::Psychic;
	} else if (type == "Bug") {
		return Util::Bug;
	} else if (type == "Rock") {
		return Util::Rock;
	} else if (type == "Ghost") {
		return Util::Ghost;
	} else if (type == "Dragon") {
		return Util::Dragon;
	} else if (type == "Dark") {
		return Util::Dark;
	} else if (type == "Steel") {
		return Util::Steel;
	} else if (type == "Fairy") {
		return Util::Fairy;
	}
	return Util::NONE;
}

string Util::getMoveTypeStringFromEnum(MoveType type) {
	switch (type) {
	case Util::Physical: return "Physical";
	case Util::Special: return "Special";
	case Util::Status: return "Status";

	default: return "That type doesn't exist";
	}
}

Util::MoveType Util::getMoveTypeEnumFromString(string type) {
	if (type == "Physical") {
		return Util::Physical;
	} else if (type == "Special") {
		return Util::Special;
	} else if (type == "Status") {
		return Util::Status;
	}
	return Util::Status;
}

vector<double> Util::getNatureFromString(string nature) {
	vector<double> changes;

	//Att Block
	if (nature == "Lonely") {
					//++					--
		changes = { NATURE_INCREASE, NATURE_DECREASE, NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD };

	} else if (nature == "Adamant") {
					//++									--
		changes = { NATURE_INCREASE, NATURE_STANDARD, NATURE_DECREASE, NATURE_STANDARD, NATURE_STANDARD };

	} else if (nature == "Naughty") {
					//++													--
		changes = { NATURE_INCREASE, NATURE_STANDARD, NATURE_STANDARD, NATURE_DECREASE, NATURE_STANDARD };

	} else if (nature == "Brave") {
					//++																	--
		changes = { NATURE_INCREASE, NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD, NATURE_DECREASE };

	//Def Block
	} else if (nature == "Bold") {
					//		--				++
		changes = { NATURE_DECREASE, NATURE_INCREASE, NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD };

	} else if (nature == "Impish") {
					//						++				--
		changes = { NATURE_STANDARD, NATURE_INCREASE, NATURE_DECREASE, NATURE_STANDARD, NATURE_STANDARD };

	} else if (nature == "Lax") {
					//						++								--
		changes = { NATURE_STANDARD, NATURE_INCREASE, NATURE_STANDARD, NATURE_DECREASE, NATURE_STANDARD };

	} else if (nature == "Relaxed") {
					//						++													--
		changes = { NATURE_STANDARD, NATURE_INCREASE, NATURE_STANDARD, NATURE_STANDARD, NATURE_DECREASE };

	//SpA Block
	} else if (nature == "Modest") {
					//		--								++
		changes = { NATURE_DECREASE, NATURE_STANDARD, NATURE_INCREASE, NATURE_STANDARD, NATURE_STANDARD };

	} else if (nature == "Mild") {
					//						--				++				
		changes = { NATURE_STANDARD, NATURE_DECREASE, NATURE_INCREASE, NATURE_STANDARD, NATURE_STANDARD };

	} else if (nature == "Rash") {
					//										++				--
		changes = { NATURE_STANDARD, NATURE_STANDARD, NATURE_INCREASE, NATURE_DECREASE, NATURE_STANDARD };

	} else if (nature == "Quiet") {
					//										++									--
		changes = { NATURE_STANDARD, NATURE_STANDARD, NATURE_INCREASE, NATURE_STANDARD, NATURE_DECREASE };

	//SpD Block
	} else if (nature == "Calm") {
				//		--													++
		changes = { NATURE_DECREASE, NATURE_STANDARD, NATURE_STANDARD, NATURE_INCREASE, NATURE_STANDARD };

	} else if (nature == "Gentle") {
				//						--									++				
		changes = { NATURE_STANDARD, NATURE_DECREASE, NATURE_STANDARD, NATURE_INCREASE, NATURE_STANDARD };

	} else if (nature == "Careful") {
				//											--				++
		changes = { NATURE_STANDARD, NATURE_STANDARD,NATURE_DECREASE, NATURE_INCREASE, NATURE_STANDARD };

	} else if (nature == "Sassy") {
				//															++				--
		changes = { NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD, NATURE_INCREASE, NATURE_DECREASE };


	//Spe Block
	} else if (nature == "Timid") {
					//		--																	++
		changes = { NATURE_DECREASE, NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD, NATURE_INCREASE};

	} else if (nature == "Hasty") {
					//						--													++				
		changes = { NATURE_STANDARD, NATURE_DECREASE, NATURE_STANDARD, NATURE_STANDARD, NATURE_INCREASE };

	} else if (nature == "Jolly") {
					//										--									++
		changes = { NATURE_STANDARD, NATURE_STANDARD, NATURE_DECREASE, NATURE_STANDARD, NATURE_INCREASE, };

	} else if (nature == "Naive") {
					//														--					++
		changes = { NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD, NATURE_DECREASE, NATURE_INCREASE, };


	} else {
		changes = {NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD, NATURE_STANDARD};

		if (nature != "Hardy" && nature != "Docile" && nature != "Bashful" && nature != "Quirky" && nature != "Serious") {
			cout << "Nature unrecognized. Using a neutral nature." << endl;
		}
	}

	return changes;
}

string Util::getStatusStringFromEnum(Util::StatusCond status) {
	if (status == Util::Standard) {
		return "Normal";
	}
	else if (status == Util::Fainted) {
		return "Fainted";
	}
	else if (status == Util::Burned) {
		return "Burned";
	}
	else if (status == Util::Frozen) {
		return "Frozen";
	}
	else if (status == Util::Paralyzed) {
		return "Paralyzed";
	}
	else if (status == Util::Poisoned) {
		return "Poisoned";
	}
	else if (status == Util::Badly_Poisoned) {
		return "Poisoned";
	} 
	else if (status == Util::Asleep) {
		return "Asleep";
	}

	return "Normal";
}

Util::Util()
{
}


Util::~Util()
{
}