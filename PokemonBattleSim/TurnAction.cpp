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

void TurnAction::setUser(Util::User newUser) {
	user = newUser;
}

void TurnAction::setAction(Util::Action newAction) {
	action = newAction;
}

void TurnAction::setParam(string newParam) {
	parameter = newParam;
}

Util::User TurnAction::getUser() {
	return user;
}

Util::Action TurnAction::getAction() {
	return action;
}

string TurnAction::getParam() {
	return parameter;
}