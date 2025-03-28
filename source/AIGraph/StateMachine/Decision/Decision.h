#pragma once
#include "AIGraph/StateMachine/StateController.h"
class StateController;
class Decision
{
public:
	Decision() {}
	virtual ~Decision() = default;

	virtual bool Decide(StateController* io_pController) = 0;

};
