#pragma once
#include "AIGraph/StateMachine/Action/Action.h"
#include "Core/Random.h"
class PatrolInAreaAction : public Action
{
public:
	PatrolInAreaAction() {}
	PatrolInAreaAction(double i_patrolTime, double i_patrolRadius,const Vec2& i_pos) :
		patrolTime(i_patrolTime),
		patrolRadius(i_patrolRadius),
		centerPos(i_pos){}
	~PatrolInAreaAction() = default;

	void Act(StateController* io_pController) override
	{
		io_pController->GetOwner().SetMoveable(true);
		time += io_pController->GetDeltaTime();
		if (time > patrolTime)
		{
			SetPatrolPos(io_pController);
			time = 0.0;
		}
	}
	bool CheckActIsCompleted(StateController* io_pController) override { return false; }
	//パトロールエリアのランダムの位置に移動
	void SetPatrolPos(StateController* io_pController)

	{
		double max_x = 19 * 32;
		double max_y = 14 * 32;
		Vec2 patrolPos = { rnd(std::max(0.0,centerPos.x - patrolRadius),std::min(centerPos.x + patrolRadius,max_x)), rnd(std::max(0.0,centerPos.y - patrolRadius),std::min(centerPos.y + patrolRadius,max_y)) };
		while (!io_pController->GetOwnerEnemy().GetAgent().GetNavMesh().isOnNavMesh(patrolPos))
		{
			patrolPos = { rnd(std::max(0.0,centerPos.x - patrolRadius),std::min(centerPos.x + patrolRadius,max_x)), rnd(std::max(0.0,centerPos.y - patrolRadius),std::min(centerPos.y + patrolRadius,max_y)) };
		}
		io_pController->GetOwnerEnemy().SetGoalPos(patrolPos);
	}

private:
	double time = 2.0;
	double patrolTime = 2.0;
	double patrolRadius = 96.0;
	Vec2 centerPos = { 0,0 };
};
