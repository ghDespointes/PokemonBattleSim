#include "MoveEffectManager.h"



MoveEffectManager::MoveEffectManager()
{
}


MoveEffectManager::~MoveEffectManager()
{
}

int MoveEffectManager::activateCritEffect(Move move) {
	int effect = move.getFunction();

	if (effect == 1 || move.getInternalName() == "RAZORWIND") {

		cout << "High Crit" << endl;
		return 1;
	}
	else if (effect == 100) {
		return 4;
	}

	return 0;
}

int MoveEffectManager::activateMultiHitEffect(Move move) {
	int effect = move.getFunction();
	string intName = move.getInternalName();
	int output = 1;
	
	if (effect == 2) {
		if (intName == "DOUBLEKICK" || intName == "TWINNEEDLE") {
			return 2;
		}

		int random = rand() % 1000;

		if (random < 333) {
			output = 2;
		}
		else if (random < 666) {
			output = 3;
		} else if (random < 833) {
			output = 4;
		} else {
			output = 5;
		}

	} else {
		
	}

	return output;
}

int MoveEffectManager::activateOHKOAcc(Move move, int *hitChance, ActivePokemon *user, ActivePokemon *victim) {
	int effect = move.getFunction();
	
	if (effect == 7) {
		*hitChance = user->getLevel() - victim->getLevel() + 30;

		if (*hitChance < 30) {
			hitChance = 0;
		}

		return true;
	}

	return false;
}

void MoveEffectManager::activate2TurnMove(Move move, ActivePokemon *user, Util::User plr) {
	int effect = move.getFunction();
	string intName = move.getInternalName();
	
	if (effect == 8) {
		if (user->getCharge() == true) {
			user->setCharge(false);
			user->setFly(false);
			user->setDig(false);
			user->resetStoredAction();

			return;
		}

		user->setCharge(true);

		if (intName == "RAZORWIND") {
			cout << user->getName() << " is whipping up a storm." << endl;
		}
		else if (intName == "FLY") {
			user->setFly(true);
			cout << user->getName() << " flew up into the sky." << endl;
		}
		else if (intName == "DIG") {
			user->setDig(true);
			cout << user->getName() << " burrowed under ground." << endl;
		}

		user->setStoredAction(TurnAction(plr, Util::Attack, intName));
	}
}

void MoveEffectManager::activateOnMiss(Move move, ActivePokemon *user) {
	int effect = move.getFunction();
	
	if (effect == 15) {
		cout << user->getName() << " continued and crashed!" << endl;
		user->damagePercent(0.5);
	}
}

void MoveEffectManager::activateRecoil(Move move, ActivePokemon *user, int damage) {
	int effect = move.getFunction();
	double recoilAmm;

	if (effect == 17) {
		recoilAmm = 1 / move.getEffChance();

		user->damage(damage * recoilAmm);
		cout << "Ya hurt yourself." << endl;
	} else if (effect == 30) {
		recoilAmm = 1 / move.getEffChance();
		user->heal(damage * recoilAmm);
		cout << "You got healed." << endl;
	}
}

void MoveEffectManager::activateForceSwitch(Move move, Team *userTeam, Team *otherTeam) {
	int effect = move.getFunction();
	int switchTo;

	if (effect == 10) {
		switchTo = (rand() % (otherTeam->getTeam().size()-1)) + 1;

		cout << otherTeam->getActive()->getName() << " was forced out!" << endl;
		otherTeam->makeActive(switchTo);
		cout << otherTeam->getActive()->getName() << " was sent out!" << endl;
		otherTeam->getActive()->setVolStatus(Util::Skip);
	}
}

void MoveEffectManager::activateFlatDamage(Move move, ActivePokemon *user, int* damage) {
	int effect = move.getFunction();

	if (effect == 23) {
		*damage = move.getEffChance();

		if (*damage == 0){
			*damage = user->getLevel();
		}

	}
}

void MoveEffectManager::activateCounter(Move move, ActivePokemon* user, int *damage) {
	int effect = move.getFunction();
	int storedDam = user->getStoredDamage();

	if (effect == 29) {

		if (storedDam != 0) {
			*damage = storedDam * 2;
		}
		else {
			*damage = 0;
			cout << "It failed..." << endl;
		}
	}
}

void MoveEffectManager::activateDamageEffect(Move move, ActivePokemon *user, ActivePokemon *victim) {
	int effect = move.getFunction();
	int chance = move.getEffChance();
	int effectChance = 100;
	bool notStatused = (victim->getStatus() == Util::Standard) ? true : false;
	
	effectChance= rand() % 100;
	
	if (effect == 3) {
		cout << "Coins are scattered around." << endl;
	}
	else if (effect == 4 && notStatused) {
		if (effectChance < chance) {
			victim->setStatus(Util::Burned);
			cout << victim->getName() << " was burned!" << endl;
		}
	}
	else if (effect == 5 && notStatused) {
		if (effectChance < chance) {
			victim->setStatus(Util::Frozen);
			cout << victim->getName() << " was frozen!" << endl;
		}
	}
	else if (effect == 6 && notStatused) {
		if (effectChance < chance) {
			victim->setStatus(Util::Paralyzed);
			cout << victim->getName() << " was paralyzed!" << endl;
		}
	}
	else if (effect == 9) {
		user->changeStatBoost(Util::AttBoost, 2);
	}
	else if (effect == 14) {
		if (effectChance < chance) {
			victim->setVolStatus(Util::Flinched);
		}
	}
	else if (effect == 16) {
		victim->changeStatBoost(Util::AccBoost, -1);
	}
	else if (effect == 28) {
		victim->changeStatBoost(Util::DefBoost, -1);
	}
	else if ((effect == 19 || move.getInternalName() == "TWINNEEDLE") && notStatused) {
		if (effectChance < chance) {
			victim->setStatus(Util::Poisoned);
			cout << victim->getName() << " was poisoned!" << endl;
		}
	}
	else if (effect == 20) {
		victim->changeStatBoost(Util::AttBoost, -1);
	}
	else if (effect == 21 && notStatused) {
		victim->setStatus(Util::Asleep);
		cout << victim->getName() << " was put to sleep!" << endl;
	}
	else if (effect == 22) {
		if (effectChance < chance) {
			victim->setVolStatus(Util::Confused);
		}
	}
	else if (effect == 24) {
		if (victim->getStoredMove() >= 0 && victim->getDisableMove() < 0) {
			victim->setDisable(victim->getStoredMove());

			cout << victim->getName() << "'s " << victim->getMoves()[victim->getStoredMove()].getName() << " was disabled!" << endl;
		}
		else {
			if (victim->getDisableMove() >= 0) {
				cout << "But a move is already disabled..." << endl;
			}
			else {
				cout << "But it failed..." << endl;
			}
		}
	}
	else if (effect == 25) {
		if (effectChance < chance) {
			victim->changeStatBoost(Util::SpDefBoost, -1);
		}
	}
	else if (effect == 27) {
		if (effectChance < chance) {
			victim->changeStatBoost(Util::SpeedBoost, -1);
		}
	}
	else if (effect == 32) {
		if (effectChance < chance) {
			victim->changeStatBoost(Util::AttBoost, 1);
			victim->changeStatBoost(Util::SpAttBoost, 1);
		}
	}
	else {
	
	}
}
