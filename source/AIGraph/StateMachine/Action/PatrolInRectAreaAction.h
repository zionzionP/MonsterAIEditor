#pragma once
#include "AIGraph/StateMachine/Action/Action.h"
#include "Core/Random.h"
class PatrolInRectAreaAction : public Action
{
public:
	PatrolInRectAreaAction() {}
	PatrolInRectAreaAction(double i_patrolTime,const Vec2& i_posTL,const Vec2& i_posBR) :
	    patrolTime(i_patrolTime),
		areaTL(i_posTL),
		areaBR(i_posBR){}
	~PatrolInRectAreaAction() = default;

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
	{	                                                                                                                                                                //max height 15  * size 32
		Vec2 patrolPos = { rnd(areaTL.x,areaBR.x), rnd(areaTL.y,areaBR.y) };
		while (!io_pController->GetOwnerEnemy().GetAgent().GetNavMesh().isOnNavMesh(patrolPos))
		{
			patrolPos = { rnd(areaTL.x,areaBR.x), rnd(areaTL.y,areaBR.y) };;
		}
		io_pController->GetOwnerEnemy().SetGoalPos(patrolPos);
	}

private:
	double time = 2.0;
	double patrolTime = 2.0;
	Vec2 areaTL = { 0,0 };
	Vec2 areaBR = { 0,0 };
};
