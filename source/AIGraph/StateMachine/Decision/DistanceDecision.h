#pragma once
#include "AIGraph/StateMachine/Decision/Decision.h"
#include "Actor/Enemy.h"
class DistanceDecision: public Decision 
{
public:
	DistanceDecision(){}
	DistanceDecision(double i_min, double i_max) : minDistance(i_min),maxDistance(i_max){}
	~DistanceDecision() = default;

	bool Decide(StateController* io_pController) override
	{
		return InDistance(io_pController);
	}
	//距離が範囲内を判断
	bool InDistance(StateController* io_pController)
	{
		Vec2 pos = io_pController->GetOwner().GetPosition();
		Vec2 otherPos = io_pController->GetOwnerEnemy().GetTarget()->GetPosition();
		double distance = pos.distanceFrom(otherPos);
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
};
