﻿#pragma once
#include "AIGraph/Editor/Node/Action/FSMGraphActionNode.h"
#include "AIGraph/StateMachine/AIFactory.h"
#include "AIGraph/StateMachine/Action/SetAroundIndexAction.h"

class FSMGraphSetAroundIndexActionNode : public FSMGraphActionNode
{
public:
	FSMGraphSetAroundIndexActionNode()
	{
		typeName_ = U"FSMGraphSetAroundIndexActionNode";
		displayName_ = U"SetPoint";
		description_ = U"ターゲット周りの8マスからランダムに1つを選ぶ";
		color_ = Palette::Blue;
		splitDescription();
	};
	~FSMGraphSetAroundIndexActionNode()override = default;

	void RegisterAction(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterAction<SetAroundIndexAction>(nodeIndex_, io_controller);
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
