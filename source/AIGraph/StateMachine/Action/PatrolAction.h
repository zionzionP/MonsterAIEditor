#pragma once
#include "AIGraph/StateMachine/Action/Action.h"
#include "Core/Random.h"

class PatrolAction : public Action
{
public:
	PatrolAction() {}
	PatrolAction(double i_patrolTime, double i_patrolRadius) :
		patrolTime(i_patrolTime),
		patrolRadius(i_patrolRadius)  {}
	~PatrolAction() = default;

	void Act(StateController* io_pController) override
	{
		time += io_pController->GetDeltaTime();
		if (time > patrolTime)
		{
			SetPatrolPos(io_pController);
			time = 0.0;
		}
	}
	bool CheckActIsCompleted(StateController* io_pController) override { return false; }
	//パトロール半径内にナビメッシュ上のランダムの位置に移動
	void SetPatrolPos(StateController* io_pController)
	{
		io_pController->GetOwner().SetMoveable(true);
		Vec2 pos = io_pController->GetOwner().GetPosition();
		double max_x = 19 * 32;
		double max_y = 14 * 32;
		Vec2 patrolPos = { rnd(std::max(0.0,pos.x - patrolRadius),std::min(pos.x + patrolRadius,max_x)), rnd(std::max(0.0,pos.y - patrolRadius),std::min(pos.y + patrolRadius,max_y)) };
		while (!io_pController->GetOwnerEnemy().GetAgent().GetNavMesh().isOnNavMesh(patrolPos))
		{
			patrolPos = { rnd(std::max(0.0,pos.x - patrolRadius),std::min(pos.x + patrolRadius,max_x)), rnd(std::max(0.0,pos.y - patrolRadius),std::min(pos.y + patrolRadius,max_y)) };
		}
		io_pController->GetOwnerEnemy().SetGoalPos(patrolPos);
		

	}

private:
	double time = 2.0;
	double patrolTime = 2.0;
	double patrolRadius = 96.0;
};
