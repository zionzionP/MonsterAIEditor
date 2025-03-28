#pragma once

class StateController;
//アクションの基底クラス
class Action
{
public:
	Action(){}
	virtual ~Action() = default;

	virtual void Act(StateController* io_pController) = 0;
	virtual bool CheckActIsCompleted(StateController* io_pController) = 0;
};
