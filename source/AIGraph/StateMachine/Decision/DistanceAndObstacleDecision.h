#pragma once
#include "AIGraph/StateMachine/Decision/Decision.h"
#include "Actor/Enemy.h"
class DistanceAndObstacleDecision : public Decision
{
public:
	DistanceAndObstacleDecision() {}
	DistanceAndObstacleDecision(double i_min, double i_max) : minDistance(i_min), maxDistance(i_max) {}
	~DistanceAndObstacleDecision() = default;

	bool Decide(StateController* io_pController) override
	{
		return InDistanceAndNoObstacle(io_pController) && !IsAnyObstacle(io_pController);
	}
	//距離が範囲内を判断
	bool InDistanceAndNoObstacle(StateController* io_pController)
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
	//ターゲットとの間に障害物があるかの判断
	bool IsAnyObstacle(StateController* io_pController)
	{
		auto& owner = io_pController->GetOwnerEnemy();
		Line line { owner.GetPosition(), owner.GetTarget()->GetPosition() };
		for (auto pActor : ActorManager::Get()->GetWalls())
		{
			CollisionComponent* pCollision = pActor->GetComponent<CollisionComponent, COLLISION>();
			if (pCollision != nullptr)
			{
				if(pCollision->Get().hitbox.intersects(line))
				{
					return true;
				}
			}
		}
		return false;
	}
private:
	double minDistance = 0.0;
	double maxDistance = 96.0;
};
