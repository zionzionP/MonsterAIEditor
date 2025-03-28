#pragma once
#pragma once
#include "AIGraph/Editor/Decision/FSMGraphDecision.h"
#include "AIGraph/StateMachine/Decision/CurrentActionDoneDecision.h"
class FSMGraphActionDoneDecision : public FSMGraphDecision
{
	typedef FSMGraphDecision super;
public:
	FSMGraphActionDoneDecision()
	{
		typeName_ = U"FSMGraphActionDoneDecision";
	};
	~FSMGraphActionDoneDecision()override = default;

	void Update() override;
	void Draw()override;

	void RegisterDecision(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterDecision<CurrentActionDoneDecision>(ownerEdge_->GetIndex(), io_controller);
	}

	nlohmann::json Serialize() const override;
	void Deserialize(const nlohmann::json& i_json) override;

private:
	double minDistance = 0.0;
	TextEditState minDistanceTe;
	double maxDistance = 0.0;
	TextEditState maxDistanceTe;

};
