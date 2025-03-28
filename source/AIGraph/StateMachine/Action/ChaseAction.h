#pragma once
#include "AIGraph/StateMachine/Action/Action.h"

class ChaseAction : public Action
{
public:
	ChaseAction(){}
	~ChaseAction() = default;

	void Act(StateController* io_pController) override
	{
		SetPlayerPos(io_pController);
	}
	bool CheckActIsCompleted(StateController* io_pController) override { return false; }
private:
	void SetPlayerPos(StateController* io_pController)
	{
		io_pController->GetOwner().SetMoveable(true);
		//プレイヤーの位置を経路探索の終点に設定
		io_pController->GetOwnerEnemy().SetGoalPos(io_pController->GetOwnerEnemy().GetTarget()->GetPosition());
	}
};
