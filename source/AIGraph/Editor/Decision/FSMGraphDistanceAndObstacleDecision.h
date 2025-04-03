#pragma once
#include "AIGraph/Editor/Decision/FSMGraphDistanceDecision.h"
#include "AIGraph/StateMachine/Decision/DistanceAndObstacleDecision.h"

class FSMGraphDistanceAndObstacleDecision : public FSMGraphDistanceDecision
{
public:
	FSMGraphDistanceAndObstacleDecision()
	{
		typeName_ = U"FSMGraphDistanceAndObstacleDecision";
	};
	~FSMGraphDistanceAndObstacleDecision()override = default;

	void RegisterDecision(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterDecision<DistanceAndObstacleDecision>(ownerEdge_->GetIndex(), io_controller, minDistance, maxDistance);
	}
};
