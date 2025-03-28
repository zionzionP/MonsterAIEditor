#pragma once
#include "AIGraph/StateMachine/StateController.h"
class StateController;
class Decision;
class State;
namespace FSM{
	struct Transition
	{
	public:
		Transition(auto i_pDecision, auto i_pTrueState, auto i_pFalseState) :
			decision(i_pDecision),
			pTrueState(i_pTrueState),
			pFalseState(i_pFalseState){}
		Transition(auto i_pDecision, auto i_pTrueState, auto i_pFalseState, int32 i_priority) :
			decision(i_pDecision),
			pTrueState(i_pTrueState),
			pFalseState(i_pFalseState),
			priority(i_priority){}
		virtual ~Transition() = default;
		
		Decision* decision;
		State* pTrueState;
		State* pFalseState;
		int32 priority = 0;
	};
}
