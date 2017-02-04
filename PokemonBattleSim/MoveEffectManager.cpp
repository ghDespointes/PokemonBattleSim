#include "MoveEffectManager.h"

//Takes a move
//Returns the additional crit levels the move applies
int MoveEffectManager::activateCritEffect(Move move) {
	int effect = move.getFunction();

	if (effect == 1 || move.getInternalName() == "RAZORWIND") {
		return 1;
	}
	//Saved for moves that will always crit
	else if (effect == 100) {
		return 4;
	}

	return 0;
}

//Determines if movs should hit multiple times
int MoveEffectManager::activateMultiHitEffect(Move move) {
	int effect = move.getFunction();
	string intName = move.getInternalName();
	int output = 1;
	
	if (effect == 2) {
		//If the move is double Kick or Twinneedle always hit twice
		if (intName == "DOUBLEKICK" || intName == "TWINNEEDLE") {
			return 2;
		}

		int random = rand() % 1000;
		//1/3 chance hitting twice, 1/3 chance hitting 3 times, 1/6 chance hitting 4 times, 1/6 chance  hitting 5 times
		if (random < Util::HIT2) {
			output = 2;
		}
		else if (random < (Util::HIT2 + Util::HIT3)) {
			output = 3;
		} else if (random < (Util::HIT2 + Util::HIT3 + Util::HIT4)) {
			output = 4;
		} else {
			output = 5;
		}

	} else {
		
	}

	return output;
}

//Takes the move, a pointer to the hit chance, the user, and the victim
int MoveEffectManager::activateOHKOAcc(Move move, int *hitChance, ActivePokemon *user, ActivePokemon *victim) {
	int effect = move.getFunction();
	
	if (effect == 7) {
		//hit chance increases as the user is higher level than the victim
		*hitChance = user->getLevel() - victim->getLevel() + Util::OHKO_OFFSET;

		//If victim is higher level, the move will always miss
		if (*hitChance < Util::OHKO_OFFSET) {
			hitChance = 0;
		}

		return true;
	}

	return false;
}

//Ignore this, not fully implemented
void MoveEffectManager::activate2TurnMove(Move move, ActivePokemon *user, Util::User plr) {
	int effect = move.getFunction();
	string intName = move.getInternalName();
	
	if (effect == 8) {
		//if a move is charged then stop and let it be used
		if (user->getCharge() == true) {
			user->setCharge(false);
			user->setFly(false);
			user->setDig(false);
			user->resetStoredAction();

			return;
		}

		user->setCharge(true);

		//Set up the move being charged
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

		//Store the action to be used next turn
		user->setStoredAction(TurnAction(plr, Util::Attack, intName));
	}
}

//Takes a move and the user
//has something happen with the user misses
void MoveEffectManager::activateOnMiss(Move move, ActivePokemon *user) {
	int effect = move.getFunction();
	
	if (effect == 15) {
		//When moves with the code 15 miss, the player loses half their health
		cout << user->getName() << " continued and crashed!" << endl;
		user->damagePercent(0.5);
	}
}

//Takes the move, the user, and the damage dealt
//Damages or heals the user, based on the damage dealt
void MoveEffectManager::activateRecoil(Move move, ActivePokemon *user, int damage) {
	int effect = move.getFunction();
	double recoilAmm;

	if (effect == 17) {
		//If code is 17 damage the user by a specified ammount
		recoilAmm = 1.0 / move.getEffChance();

		user->damage(damage * recoilAmm);
		cout << user->getName() << "took some recoil damage." << endl;
	} else if (effect == 30) {
		//If code is 30 heal the user by a specified ammount
		recoilAmm = 1.0 / move.getEffChance();
		user->heal(damage * recoilAmm);

		cout << user->getName() << " healed some damage." << endl;
	}
}

//Takes a move, the user's team, and other's team
void MoveEffectManager::activateForceSwitch(Move move, Team *userTeam, Team *otherTeam) {
	int effect = move.getFunction();
	int switchTo;

	if (effect == 10) {
		//Determine what pokemon to randomly switch to
		switchTo = (rand() % (otherTeam->getTeam().size()-1)) + 1;

		cout << otherTeam->getActive()->getName() << " was forced out!" << endl;
		otherTeam->makeActive(switchTo);
		cout << otherTeam->getActive()->getName() << " was sent out!" << endl;

		//If somehow the enemy didn't perform their action yet stop it from happening
		otherTeam->getActive()->setVolStatus(Util::Skip);
	}
}

//Takes the move, the user, and the damage pointer
void MoveEffectManager::activateFlatDamage(Move move, ActivePokemon *user, int* damage) {
	int effect = move.getFunction();

	if (effect == 23) {
		//If the enemy is immune to the type then damage will be 0
		if (*damage == 0) {
			return;
		}

		//Edits the damage pointer to instead be what the flat damage should be
		*damage = move.getEffChance();

		if (*damage == 0){
			//Do damage based on level if the damage is negative
			*damage = user->getLevel();
		}

	}
}

//Takes a move, use, and damage pointer
void MoveEffectManager::activateCounter(Move move, ActivePokemon* user, int *damage) {
	int effect = move.getFunction();
	int storedDam = user->getStoredDamage();

	if (effect == 29) {
		//If the enemy is immune to the type then damage will be 0
		if (*damage == 0) {
			return;
		}

		//As long as the stored damage isn't zero double it and deal it back
		if (storedDam != 0) {
			*damage = storedDam * 2;
		}
		else {
			//If damage is zero (because no physical damage was done this turn) it fails
			*damage = 0;
			cout << "It failed..." << endl;
		}
	}
}

//Takes the move, user, and victim
//Acts as a catch all for any addional effects that may occur
void MoveEffectManager::activateDamageEffect(Move move, ActivePokemon *user, ActivePokemon *victim) {
	int effect = move.getFunction();
	int chance = move.getEffChance();
	int effectChance = 100;
	Util::StatusCond statusCond = victim->getStatus();
	
	effectChance= rand() % 100;
	
	//Payday scatters coins around (Has no practical effect)
	if (effect == 3) {
		cout << "Coins are scattered around." << endl;
	}
	//These potentially burn the victim
	else if (effect == 4) {
		if (effectChance < chance) {
			if (statusCond == Util::Standard && !victim->isType(Util::Fire)) {
				victim->setStatus(Util::Burned);
				cout << victim->getName() << " was burned!" << endl;
			}
			else {
				if (move.getMoveType() == Util::Status) {
					if (statusCond == Util::Burned) {
						cout << victim->getName() << " is already burned!" << endl;
					}
					else {
						cout << "But it failed..." << endl;
					}
				}
			}
		}
	}
	//These potentially freeze the victim
	else if (effect == 5) {
		if (effectChance < chance) {
			if (statusCond == Util::Standard && !victim->isType(Util::Ice)) {
				victim->setStatus(Util::Frozen);
				cout << victim->getName() << " was frozen!" << endl;
			}
			else {
				if (move.getMoveType() == Util::Status) {
					if (statusCond == Util::Frozen) {
						cout << victim->getName() << " is already frozen!" << endl;
					}
					else {
						cout << "But it failed..." << endl;
					}
				}
			}
		}
	}
	//These potentially paralyze the victim
	else if (effect == 6) {
		if (statusCond == Util::Standard && !victim->isType(Util::Electric)) {
			victim->setStatus(Util::Paralyzed);
			cout << victim->getName() << " was paralyzed!" << endl;
		}
		else {
			if (move.getMoveType() == Util::Status) {
				if (statusCond == Util::Paralyzed) {
					cout << victim->getName() << " is already paralyzed!" << endl;
				}
				else {
					cout << "But it failed..." << endl;
				}
			}
		}
	}
	//These boost the user's attack
	else if (effect == 9) {
		user->changeStatBoost(Util::AttBoost, 2);
	}
	//These potentially flinch the victim
	else if (effect == 14) {
		if (effectChance < chance) {
			victim->setVolStatus(Util::Flinched);
		}
	}
	//These decrease the victim's accuracy
	else if (effect == 16) {
		victim->changeStatBoost(Util::AccBoost, -1);
	}
	//These decrease the victim's defense
	else if (effect == 28) {
		victim->changeStatBoost(Util::DefBoost, -1);
	}
	//These potentially poison the victim
	//Note that twinneedle is hard coded because it is using the function code for being a multi-hit move
	else if ((effect == 19 || move.getInternalName() == "TWINNEEDLE")) {
		if (statusCond == Util::Standard && !victim->isType(Util::Poison)) {
			victim->setStatus(Util::Poisoned);
			cout << victim->getName() << " was poisoned!" << endl;
		}
		else {
			if (move.getMoveType() == Util::Status) {
				if (statusCond == Util::Poisoned || statusCond == Util::Badly_Poisoned) {
					cout << victim->getName() << " is already poisoned!" << endl;
				}
				else {
					cout << "But it failed..." << endl;
				}
			}
		}
	}
	//These decrease the victim's attack
	else if (effect == 20) {
		victim->changeStatBoost(Util::AttBoost, -1);
	}
	//These potentially put the victim to sleep
	else if (effect == 21) {
		if (statusCond == Util::Standard) {
			victim->setStatus(Util::Asleep);
			cout << victim->getName() << " fell asleep!" << endl;
		}
		else {
			if (move.getMoveType() == Util::Status) {
				if (statusCond == Util::Asleep) {
					cout << victim->getName() << " is already asleep!" << endl;
				}
				else {
					cout << "But it failed..." << endl;
				}
			}
		}
	}
	//These potentially confuse the victim
	else if (effect == 22) {
		if (effectChance < chance) {
			victim->setVolStatus(Util::Confused);
		}
	}
	//These potentially disable the last move the victim used
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
	//These decrease the victim's special defense
	else if (effect == 25) {
		if (effectChance < chance) {
			victim->changeStatBoost(Util::SpDefBoost, -1);
		}
	}
	//These decrease the victim's speed
	else if (effect == 27) {
		if (effectChance < chance) {
			victim->changeStatBoost(Util::SpeedBoost, -1);
		}
	}
	//These increase the user's attack and special attack
	else if (effect == 32) {
		if (effectChance < chance) {
			user->changeStatBoost(Util::AttBoost, 1);
			user->changeStatBoost(Util::SpAttBoost, 1);
		}
	}
	else {
	
	}
}
