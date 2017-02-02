#pragma once
#include "Util.h"

//Very simple class used to identify what is meant to happen in a turn
class TurnAction
{
	//Who is performing the action
	Util::User user;

	//What the action actually is
	Util::Action action;

	//Any necesary parameter
	string parameter;

public:
	//Constructor/Destructor
	TurnAction();
	TurnAction(Util::User usr, Util::Action act, string param);
	~TurnAction();

	//Getters and setters
	void setUser(Util::User newUser);
	void setAction(Util::Action newAction);
	void setParam(string newParam);

	Util::User getUser();
	Util::Action getAction();
	string getParam();
};

