#pragma once
#include "AIGraph/StateMachine/Action/Action.h"
#include "Core/Random.h"

class SetAroundIndexAction : public Action
{
public:
	SetAroundIndexAction() {}
	~SetAroundIndexAction() = default;

	void Act(StateController* io_pController) override
	{
		isDone = false;
		SetAroundIndex(io_pController);
	}
	bool CheckActIsCompleted(StateController* io_pController) override { return isDone; }

	void SetAroundIndex(StateController* io_pController)
	{		
		//プレイヤーの周り1マスの位置をランダムで選定
		int index = rnd(0, 7);
		io_pController->GetOwnerEnemy().SetAroundPosIndex(index);
		isDone = true;
	}
private:
	bool isDone = false;
};
