#pragma once
#include "AIGraph/StateMachine/Action/Action.h"
class ChaseAroundAction : public Action
{
public:
	ChaseAroundAction() {}
	~ChaseAroundAction() = default;

	void Act(StateController* io_pController) override
	{
		SetAroundPos(io_pController);
	}
	bool CheckActIsCompleted(StateController* io_pController) override { return false; }
private:
	void SetAroundPos(StateController* io_pController)
	{
		//プレイヤーの周り1マスの位置を経路探索の終点に設定
		io_pController->GetOwner().SetMoveable(true);
		int index = io_pController->GetOwnerEnemy().GetAroundPosIndex();
		Vec2 playerPos = io_pController->GetOwnerEnemy().GetTarget()->GetPosition();
		Vec2 aroundPos = playerPos + Common::aroundPoint[index];
		while(!io_pController->GetOwnerEnemy().GetAgent().GetNavMesh().isOnNavMesh(aroundPos))
		{
			int newIndex = rnd(0, 7);
			io_pController->GetOwnerEnemy().SetAroundPosIndex(newIndex);
			aroundPos = playerPos + Common::aroundPoint[newIndex];
		}
		io_pController->GetOwnerEnemy().SetGoalPos(aroundPos);
		Vec2 apNavMeshPos = Common::PixelPosToNavMeshPos(aroundPos);
		if(Common::PixelPosToNavMeshPos(io_pController->GetOwner().GetPosition()) == apNavMeshPos)
		{
			io_pController->GetOwnerEnemy().SetGoalPos(io_pController->GetOwnerEnemy().GetTarget()->GetPosition());
		}
	}
};
