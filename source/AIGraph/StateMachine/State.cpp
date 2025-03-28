#include "AIGraph/StateMachine/State.h"

void State::UpdateState(StateController* io_pController)
{
	io_pController->SetCurrentStateActionDone(false);
	DoAction(io_pController);
	CheckTransition(io_pController);
}

void State::DoEnter(StateController* io_pController)
{
	for (Action* pAction : enterActions_)
	{
		pAction->Act(io_pController);
	}
}

void State::DoExit(StateController* io_pController)
{
	for (Action* pAction : exitActions_)
	{
		pAction->Act(io_pController);
	}
}

void State::DoAction(StateController* io_pController)
{
	for (Action* pAction : actions_)
	{
		pAction->Act(io_pController);
		io_pController->SetCurrentStateActionDone(pAction->CheckActIsCompleted(io_pController));
	}
}
void State::CheckTransition(StateController* io_pController)
{
	for (const FSM::Transition& transition : transitions_)
	{
		if (transition.decision->Decide(io_pController))
		{
			io_pController->TransitionToState(transition.pTrueState);
			break;
		}
		io_pController->TransitionToState(transition.pFalseState);		
	}
}

