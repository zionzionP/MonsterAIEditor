#pragma once
#include "AIGraph/Editor/Decision/FSMGraphDecision.h"
#include "AIGraph/StateMachine/Decision/DistanceDecision.h"

class FSMGraphDistanceDecision : public FSMGraphDecision
{
	typedef FSMGraphDecision super;
public:
	FSMGraphDistanceDecision()
	{
		typeName_ = U"FSMGraphDistanceDecision";
	};
	~FSMGraphDistanceDecision()override = default;

	void Update() override;
	void Draw()override;

	void RegisterDecision(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterDecision<DistanceDecision>(ownerEdge_->GetIndex(), io_controller, minDistance, maxDistance);
	}

	void DebugDraw(Actor* pActor) override;

	nlohmann::json Serialize() const override;
	virtual void Deserialize(const nlohmann::json& i_json);	

protected:
	double minDistance = 0.0;
	TextEditState minDistanceTe;
	double maxDistance = 0.0;
	TextEditState maxDistanceTe;

};
