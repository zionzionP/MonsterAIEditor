#pragma once
#include "AIGraph/StateMachine/Action/Action.h"

class AttackAction : public Action
{
public:
	AttackAction() {}
	~AttackAction() = default;

	void Act(StateController* io_pController) override
	{
		Attack(io_pController);
	}

	bool CheckActIsCompleted(StateController* io_pController) override { return false; }
private:
	void Attack(StateController* io_pController)
	{
		if (!io_pController->GetOwnerEnemy().GetIsAttacking())
		{
			//攻撃の方向を設定、フラグをオンにする
			Vec2 pos = io_pController->GetOwner().GetPosition();
			Vec2 otherPos = io_pController->GetOwnerEnemy().GetTarget()->GetPosition();
			Vec2 diff = otherPos - pos;
			io_pController->GetOwnerEnemy().SetAttackVec2(diff / diff.length());
			io_pController->GetOwner().SetMoveable(false);
			io_pController->GetOwnerEnemy().SetIsAttacking(true);
		}		
	}
};
