#pragma once
#include "AIGraph/Editor/Node/Action/FSMGraphActionNode.h"
#include "AIGraph/StateMachine/AIFactory.h"
#include "AIGraph/StateMachine/Action/IdleAction.h"

class FSMGraphWaitActionNode : public FSMGraphActionNode
{
public:
	FSMGraphWaitActionNode()
	{
		typeName_ = U"FSMGraphWaitActionNode";
		displayName_ = U"Wait";
		color_ = Palette::Purple;
	};
	~FSMGraphWaitActionNode()override = default;

	void RegisterAction(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterAction<IdleAction>(nodeIndex_, io_controller,waitTime);
	}

	void UpdateProperty() override
	{
		font(U"待機時間(s)").draw(25, Vec2{ 1345, 100 + 40 }, Palette::Black);
		font(U"0の場合、無限待機").draw(25, Vec2{ 1345, 100 + 120 }, Palette::Black);
		SimpleGUI::TextBox(waitTimeTe, Vec2{ 1340 + 5, 100 + 80 }, 50);
		waitTime = waitTimeTe.text.any() ? std::stod(waitTimeTe.text.narrow()) : waitTime;
	}

	nlohmann::json Serialize() const override
	{
		return { "node", {
			{"typeName", typeName_.narrow()},
			{"index", nodeIndex_},
			{"position", {position_.x, position_.y}},
			{"waitTime", waitTime}
			}
		};
	}

	void Deserialize(const nlohmann::json& i_json) override
	{
		nodeIndex_ = i_json["index"];
		position_ = { i_json["position"][0] , i_json["position"][1] };
		waitTime = i_json["waitTime"];
		waitTimeTe.text = Unicode::Widen(Common::doubleToString(waitTime));
	}
private:
	double waitTime = 2.0;
	TextEditState waitTimeTe;

};
