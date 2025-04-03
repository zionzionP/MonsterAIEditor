#pragma once
#include "AIGraph/Editor/Node/Action/FSMGraphActionNode.h"
#include "AIGraph/StateMachine/AIFactory.h"
#include "AIGraph/StateMachine/Action/AttackAction.h"

class FSMGraphAttackActionNode : public FSMGraphActionNode
{
public:
	FSMGraphAttackActionNode()
	{
		typeName_ = U"FSMGraphAttackActionNode";
		displayName_ = U"Attack";
		description_ = U"ターゲットに攻撃";
		color_ = Palette::Red;
		splitDescription();
	};
	~FSMGraphAttackActionNode()override = default;

	void RegisterAction(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterAction<AttackAction>(nodeIndex_, io_controller);
	}

	nlohmann::json Serialize() const override
	{
		return { "node",{
			{"typeName", typeName_.narrow()},
			{"index", nodeIndex_},
			{"position", {position_.x, position_.y}}
			}
		};
	}
	void Deserialize(const nlohmann::json& i_json) override
	{
		nodeIndex_ = i_json["index"];
		position_ = { i_json["position"][0] , i_json["position"][1]};
	}
};
