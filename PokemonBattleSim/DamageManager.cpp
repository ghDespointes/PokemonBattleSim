#include "DamageManager.h"

//Sets the whole type effectivness chart
const vector<vector<double>> DamageManager::typeEffectiveness = {
	//To get attack effectiveness:		typeEffectiveness[AttackType][DefenseType]
	//To get defensive effectiveness:	typeEffectiveness[DefenseType][AttackType]

	//Magic number are bad, but it would get ridiculous very quickly here

	//Normal
	{ 1, 1, 1, 1, 1, 0.5, 1, 0, 0.5, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	//Fighting
	{ 2, 1, 0.5, 0.5, 1, 2, 0.5, 0, 2, 1, 1, 1, 1, 0.5, 2, 1, 2, 0.5 },
	//Flying
	{ 1, 2, 1, 1, 1, 0.5, 2, 1, 0.5, 1, 1, 2, 0.5, 1, 1, 1, 1, 1 },
	//Poison
	{ 1, 1, 1, 0.5, 0.5, 0.5, 1, 0.5, 0, 1, 1, 2, 1, 1, 1, 1, 1, 2 },
	//Ground
	{ 1, 1, 0, 2, 1, 2, 0.5, 1, 2, 2, 1, 0.5, 2, 1, 1, 1, 1, 1 },
	//Rock
	{ 1, 0.5, 2, 1, 0.5, 1, 2, 1, 0.5, 2, 1, 1, 1, 1, 2, 1, 1, 1 },
	//Bug
	{ 1, 0.5, 0.5, 0.5, 1, 1, 1, 0.5, 0.5, 0.5, 1, 2, 1, 2, 1, 1, 2, 0.5 },
	//Ghost
	{ 0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 0.5, 1 },
	//Steel
	{ 1, 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5, 0.5, 1, 0.5, 1, 2, 1, 1, 2 },
	//Fire
	{ 1, 1, 1, 1, 1, 0.5, 2, 1, 2, 0.5, 0.5, 2, 1, 1, 2, 0.5, 1, 1 },
	//Water
	{ 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 1, 0.5, 1, 1 },
	//Grass
	{ 1, 1, 0.5, 0.5, 2, 2, 0.5, 1, 0.5, 0.5, 2, 0.5, 1, 1, 1, 0.5, 1, 1 },
	//Electric
	{ 1, 1, 2, 1, 0, 1, 1, 1, 1, 1, 2, 0.5, 0.5, 1, 1, 0.5, 1, 1 },
	//Psychic
	{ 1, 2, 1, 2, 1, 1, 1, 1, 0.5, 1, 1, 1, 1, 0.5, 1, 1, 0, 1 },
	//Ice
	{ 1, 1, 2, 1, 2, 1, 1, 1, 0.5, 0.5, 0.5, 2, 1, 1, 0.5, 2, 1, 1 },
	//Dragon
	{ 1, 1, 1, 1, 1, 1, 1, 1, 0.5, 1, 1, 1, 1, 1, 1, 2, 1, 0 },
	//Dark
	{ 1, 0.5, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 2, 1, 1, 0.5, 0.5 },
	//Fairy
	{ 1, 2, 1, 0.5, 1, 1, 1, 1, 0.5, 0.5, 1, 1, 1, 1, 1, 2, 2, 1 },
};

//Takes the attacking type and the defending type
//Returns the value from type effectiveness matrix
double DamageManager::getEffectiveness(Util::PokeType type1, Util::PokeType type2) {
	return typeEffectiveness[type1][type2];
}

//Takes the type of the attack and the defending pokemon
//Calculates the full effectiveness of moves against pokemon with multiple types
double DamageManager::getMoveEffectiveness(Util::PokeType moveType, Pokemon victim) {
	double damageMultiplier = 1;
	vector<Util::PokeType> types = victim.getType();

	for (int i = 0; i < types.size(); i++) {
		if (types[i] != Util::NONE) {
			damageMultiplier *= getEffectiveness(moveType, types[i]);
		}
	}

	return damageMultiplier;
}

//Takes the move, the user, the defender, and if the move crits
//Calculates any modifiers
double DamageManager::getModifier(Move move, Pokemon user, Pokemon victim, bool crit) {
	double stab = 1;
	double critVal = 1;
	double type = getMoveEffectiveness(move.getType(), victim);
	//Moves deal anywhere from .85 to 1 of their damage
	double ran = (double)(100 - rand() % 16) / 100;

	//Reserved for future abilities and items
	double other = 1;

	//Crits increase damage by 1.5
	if (crit && type != 0) {
		critVal = 1.5;
		cout << "It's a critical hit! ";
	}

	if (type > 1) {
		cout << "It's super effective! ";
	} else if (type == 0) {
		cout << "It had no effect! ";
	} else if (type < 1) {
		cout << "It's not very effective! ";
	}

	//Moves deal more damage if they share a type with the user
	vector<Util::PokeType> types = user.getType();
	for (int i = 0; i < types.size(); i++) {
		if (types[i] == move.getType()) {
			stab = 1.5;
		}
	}

	return stab * critVal * type * ran;
}

//Takes the move, the attacker, the defender, and if the move crits or not
//Calculate base damage 
double DamageManager::calcBaseDamage(Move move, ActivePokemon attacker, ActivePokemon defender, bool crit) {
	int basePower = move.getBP();
	Util::MoveType moveType = move.getMoveType();

	int lvl = attacker.getLevel();
	int attack = 0;
	int defense = 0;

	double damage = 0;

	double attackBoost;
	double defenseBoost;

	//If the move is physical only use the physical stats (Attack and defense)
	if (moveType == Util::Physical) {
		attack = attacker.getStat(Util::Att);
		defense = defender.getStat(Util::Def);

		//Get any modifiers
		attackBoost = attacker.getStatBoost(Util::AttBoost);
		defenseBoost = defender.getStatBoost(Util::DefBoost);

		//if its a crit ignore any negative modifiers for the attack and ignore positive modifiers for the defense
		if (crit) {
			if (attackBoost > 1) {
				attack *= attackBoost;
			}

			if (defenseBoost < 1) {
				defense *= defenseBoost;
			}
		}
		else {
			attack *= attackBoost;
			defense *= defenseBoost;
		}

		//If the attack is burned the damage they deal is halved
		if (attacker.getStatus() == Util::Burned) {
			attack = attack / 2;
		}
	} 
	//If the move is physical, only use the Special stats (Special Attack and special defense)
	else {
		attack = attacker.getStat(Util::SpAtt);
		defense = defender.getStat(Util::SpDef);

		//Get any modifiers
		attackBoost = attacker.getStatBoost(Util::SpAttBoost);
		defenseBoost = defender.getStatBoost(Util::SpDefBoost);

		//if its a crit ignore any negative modifiers for the attack and ignore positive modifiers for the defense
		if (crit) {
			if (attackBoost > 1) {
				attack *= attackBoost;
			}

			if (defenseBoost < 1) {
				defense *= defenseBoost;
			}
		}
		else {
			attack *= attackBoost;
			defense *= defenseBoost;
		}

		//Special attacks are unaffected by burns
	}

	//Calculate the damage based on pokemon formulas
	damage = 2 * lvl + 10;
	damage = damage / 250;
	damage *= basePower;
	damage *= ((double)attack / (double)defense);
	damage += 2;

	//Return the final base damage rounded down
	return (int)damage;

}

//Calculate the damage taken from hitting yourself in confusion
int DamageManager::calcConfusion(ActivePokemon user) {
	double baseDamage;
	Move move = Util::getMove("CONFUSEDHIT");

	baseDamage = calcBaseDamage(move, user, user, false);

	return baseDamage;
}

//Takes the move, user, and defender
//Return the damage they take from that interaction
int DamageManager::calcDamage(ActiveMove move, ActivePokemon user, ActivePokemon victim) {
	Move moveInfo = Util::getMove(move.getIntName());
	Pokemon attacking = Util::getPoke(user.getIntName());
	Pokemon defending = Util::getPoke(victim.getIntName());
	int critChance = 1;
	bool crit;

	double baseDamage;
	double modifier;

	//Sees if critical hit ratios are affected by the move being used
	critChance += MoveEffectManager::activateCritEffect(moveInfo);

	critChance += user.getStatBoost(Util::CritBoost) * 16;

	//Calculates if the move is a critical hit or not
	crit = (rand() % 16) < critChance ? true : false;

	//Calculates base damage and modifier
	baseDamage = calcBaseDamage(moveInfo, user, victim, crit);
	modifier = getModifier(moveInfo, attacking, defending, crit);

	//Returns the final damage
	return baseDamage*modifier;
}
