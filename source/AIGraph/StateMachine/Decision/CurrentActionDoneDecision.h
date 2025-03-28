#pragma once
#include "AIGraph/StateMachine/Decision/Decision.h"
class CurrentActionDoneDecision : public Decision
{
public:
	CurrentActionDoneDecision() {}
	~CurrentActionDoneDecision() = default;

	bool Decide(StateController* io_pController) override
	{
		return io_pController->GetIsCurrentStateActionDone();
	}
};
