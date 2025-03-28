#include "AIGraph/StateMachine/StateController.h"

void StateController::Update(double i_deltaTime)
{
	deltaTime_ = i_deltaTime;
	pCurrentState_->UpdateState(this);
}

void StateController::TransitionToState(State* i_nextState)
{
	if (i_nextState != nullptr && i_nextState != pCurrentState_)
	{
		pCurrentState_->DoExit(this);
		pCurrentState_ = i_nextState;
		pCurrentState_->DoEnter(this);
	}
}
