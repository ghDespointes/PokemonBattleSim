#include "AIManager.h"



AIManager::AIManager(Team* inputTeam, Team* othTeam){
	team = inputTeam;
	otherTeam = othTeam;
}


AIManager::~AIManager()
{
}

//Very simply gives an approximation of the effectiveness of the move against the enemy's active pokemon
//The more effective the higher the return value
double AIManager::calculateMoveEffectiveness(ActiveMove move) {
	double effectiveness;
	Pokemon active = Util::getPoke(team->getActive()->getIntName());
	Pokemon enemy = Util::getPoke(otherTeam->getActive()->getIntName());
	Move moveInfo = Util::getMove(move.getIntName());

	if (move.getPP() == 0) {
		return 0;
	}

	effectiveness = DamageManager::getMoveEffectiveness(moveInfo.getType(), enemy);
	effectiveness *= DamageManager::checkStab(moveInfo.getType(), active);

	return effectiveness;
}

//Determines how weak the pokemon is compared the enemy's active pokemon
//The weaker they are the lower the return value
//Therefore AI will prefer making the safer switch
double AIManager::calculateSwitchEffectiveness(ActivePokemon poke) {
	Pokemon switchTo = Util::getPoke(poke.getIntName());
	Pokemon enemyPoke = Util::getPoke(otherTeam->getActive()->getIntName());
	vector<Util::PokeType> enemyTypes = enemyPoke.getType();
	vector<Util::PokeType> types = switchTo.getType();

	double effectiveness = 1;

	if (poke.getStatus() == Util::Fainted) {
		return 0;
	}

	//Multiply by how the effectiveness your types are compared to theirs
	for (int i = 0; i < types.size(); i++) {
		effectiveness *= DamageManager::getMoveEffectiveness(types[i], enemyPoke);
	}

	//Divide by how effective their types are compared to yours
	for (int i = 0; i < enemyTypes.size(); i++) {
		effectiveness /= DamageManager::getMoveEffectiveness(enemyTypes[i], switchTo);
	}

	return effectiveness;
}

//Calculates which move will currently deal the most damage
int AIManager::calculateBestMove() {
	ActivePokemon* active = team->getActive();
	vector<ActiveMove> moves = active->getMoves();
	vector<double> moveEffects;
	Move move;
	Util::MoveType type;

	double reserve = 0;
	int winningMove = 0;

	//Determine which move will be most effective
	for (int i = 0; i < moves.size(); i++) {
		move = Util::getMove(moves[i].getIntName());
		moveEffects.push_back(0);

		type = move.getMoveType();

		//If it deals damage determine and idea of how much damage ach will do
		if (type != Util::Status) {
			moveEffects[i] = calculateMoveEffectiveness(moves[i]);
			moveEffects[i] *= move.getBP();

			if (type == Util::Physical) {
				moveEffects[i] *= active->getStat(Util::Att) * active->getStatBoost(Util::AttBoost);
			}
			else {
				moveEffects[i] *= active->getStat(Util::SpAtt) * active->getStatBoost(Util::SpAttBoost);
			}

		}
		//Status moves will will get complicated quickly
		else {
		
		}
	}

	//See which move is the most effective
	for (int i = 0; i < moveEffects.size(); i++) {
		if (moveEffects[i] > reserve) {
			reserve = moveEffects[i];
			winningMove = i;
		}
	}

	return winningMove;
}

int AIManager::calculateBestSwitch() {
	vector<ActivePokemon> pokemon = team->getTeam();
	vector<double> pokeEffects;
	Pokemon poke;
	Util::MoveType type;

	double reserve = 0;
	int winningPoke = 0;

	//Determine which move will be most effective
	for (int i = 0; i < pokemon.size(); i++) {
		pokeEffects.push_back(0);

		pokeEffects[i] = calculateSwitchEffectiveness(pokemon[i]);
	}

	//See which move is the most effective
	for (int i = 0; i < pokeEffects.size(); i++) {
		if (pokeEffects[i] > reserve) {
			reserve = pokeEffects[i];
			winningPoke = i;
		}
	}

	return winningPoke;
}

TurnAction AIManager::determineAction(Util::Action act) {
	int bestAction = 0;
	
	if (act == Util::Switch) {
		bestAction = calculateBestSwitch();

		return TurnAction(Util::Enemy_1, Util::Switch, to_string(bestAction));
	}
	else {
		bestAction = calculateBestMove();

		return TurnAction(Util::Enemy_1, Util::Attack, to_string(bestAction));
	}
}
