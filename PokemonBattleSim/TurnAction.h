#pragma once
#include "Util.h"

class TurnAction
{
	Util::User user;
	Util::Action action;
	string parameter;

public:
	TurnAction();
	TurnAction(Util::User usr, Util::Action act, string param);
	~TurnAction();

	void setUser(Util::User newUser);
	void setAction(Util::Action newAction);
	void setParam(string newParam);

	Util::User getUser();
	Util::Action getAction();
	string getParam();
};

