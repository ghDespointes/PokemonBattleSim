#include "TurnAction.h"



TurnAction::TurnAction() {

}

TurnAction::TurnAction(Util::User usr, Util::Action act, string param) {
	user = usr;
	action = act;
	parameter = param;
}

TurnAction::~TurnAction() {

}

//Set user
void TurnAction::setUser(Util::User newUser) {
	user = newUser;
}

//Set action
void TurnAction::setAction(Util::Action newAction) {
	action = newAction;
}

//Set parameter
void TurnAction::setParam(string newParam) {
	parameter = newParam;
}

//Get user
Util::User TurnAction::getUser() {
	return user;
}

//Get action
Util::Action TurnAction::getAction() {
	return action;
}

//Get parameter
string TurnAction::getParam() {
	return parameter;
}