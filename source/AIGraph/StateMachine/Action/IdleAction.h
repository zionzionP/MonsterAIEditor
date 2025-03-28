#pragma once
#include "AIGraph/StateMachine/Action/Action.h"

class IdleAction : public Action
{
public:
	IdleAction(double i_waitTime):waitTime(i_waitTime) {}
	~IdleAction() = default;

	void Act(StateController* io_pController) override
	{
		StayHere(io_pController);
	}
	bool CheckActIsCompleted(StateController* io_pController) override
	{
		time += io_pController->GetDeltaTime();
		if (time >= waitTime)
		{
			time = 0.0;
			return true;
		}
		return false;
	}

	void StayHere(StateController* io_pController)
	{
		io_pController->GetOwner().SetMoveable(false);
		io_pController->GetOwnerEnemy().SetIsAttacking(false);
	}

private:
	double time = 0.0;
	double waitTime = 2.0;
};
