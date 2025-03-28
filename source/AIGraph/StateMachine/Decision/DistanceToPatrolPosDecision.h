#pragma once
#pragma once
#include "AIGraph/StateMachine/Decision/Decision.h"
#include "Actor/Enemy.h"
class DistanceToPatrolPosDecision : public Decision
{
public:
	DistanceToPatrolPosDecision() {}
	DistanceToPatrolPosDecision(double i_min, double i_max, const Vec2& i_pos) : minDistance(i_min), maxDistance(i_max),centerPos(i_pos)  {}
	~DistanceToPatrolPosDecision() = default;

	bool Decide(StateController* io_pController) override
	{
		return InDistance(io_pController);
	}
	//プレイヤーがパトロール範囲内を判断
	bool InDistance(StateController* io_pController)
	{
		Vec2 pos = io_pController->GetOwnerEnemy().GetTarget()->GetPosition();
		double distance = pos.distanceFrom(centerPos);
		if (distance >= minDistance && distance < maxDistance)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
private:
	double minDistance = 0.0;
	double maxDistance = 96.0;
	Vec2 centerPos = { 0,0 };
};
