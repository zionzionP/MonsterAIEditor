#pragma once
#include "AIGraph/Editor/Node/Action/FSMGraphActionNode.h"
#include "AIGraph/StateMachine/AIFactory.h"
#include "AIGraph/StateMachine/Action/ChaseAction.h"

class FSMGraphChaseActionNode : public FSMGraphActionNode
{
public:
	FSMGraphChaseActionNode()
	{
		typeName_ = U"FSMGraphChaseActionNode";
		displayName_ = U"Chase";
		color_ = Palette::Green;
	};
	~FSMGraphChaseActionNode()override = default;

	void RegisterAction(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterAction<ChaseAction>(nodeIndex_, io_controller);
	}

	nlohmann::json Serialize() const override
	{
		return { "node", {
			{"typeName", typeName_.narrow()},
			{"index", nodeIndex_},
			{"position", {position_.x, position_.y}}
			}
		};
	}

	void Deserialize(const nlohmann::json& i_json) override
	{
		nodeIndex_ = i_json["index"];
		position_ = { i_json["position"][0] , i_json["position"][1] };
	}
};
