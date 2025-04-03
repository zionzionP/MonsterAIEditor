#pragma once
#include "AIGraph/Editor/Node/Action/FSMGraphActionNode.h"
#include "AIGraph/StateMachine/AIFactory.h"
#include "AIGraph/StateMachine/Action/ChaseAroundAction.h"

class FSMGraphChaseAroundActionNode : public FSMGraphActionNode
{
public:
	FSMGraphChaseAroundActionNode()
	{
		typeName_ = U"FSMGraphChaseAroundActionNode";
		displayName_ = U"ChaseAround";
		description_ = U"ターゲット周りの8マス中の1マスに追跡";
		color_ = Palette::Green;
		splitDescription();
	};
	~FSMGraphChaseAroundActionNode()override = default;

	void RegisterAction(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterAction<ChaseAroundAction>(nodeIndex_, io_controller);
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
