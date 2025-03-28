#pragma once
#include "AIGraph/Editor/Node/Action/FSMGraphActionNode.h"
#include "AIGraph/StateMachine/AIFactory.h"
#include "AIGraph/StateMachine/Action/PatrolAction.h"
class FSMGraphPatrolActionNode : public FSMGraphActionNode
{
public:
	FSMGraphPatrolActionNode()
	{
		displayName_ = U"Patrol";
		typeName_ = U"FSMGraphPatrolActionNode";
		color_ = Palette::Green;
	};
	~FSMGraphPatrolActionNode()override = default;

	void RegisterAction(StateController* io_controller) override
	{
		AIFactory::Get()->RegisterAction<PatrolAction>(nodeIndex_, io_controller, patrolTime, patrolRadius);
	}

	void UpdateProperty() override
	{
		font(U"パトロール間隔(s)").draw(25, Vec2{ 1345, 100 + 40 }, Palette::Black);
		font(U"パトロール半径").draw(25, Vec2{ 1345, 100 + 120 }, Palette::Black);
		SimpleGUI::TextBox(patrolTimeTe, Vec2{ 1340 + 5, 100 + 80 }, 50);
		SimpleGUI::TextBox(patrolRadiusTe, Vec2{ 1340 + 5, 100 + 160 }, 50);
		patrolTime = patrolTimeTe.text.any()? std::stod(patrolTimeTe.text.narrow()) : patrolTime;
		patrolRadius = patrolRadiusTe.text.any() ? std::stod(patrolRadiusTe.text.narrow()) : patrolRadius;
	}

	nlohmann::json Serialize() const override
	{
		return { "node", {
			{"typeName", typeName_.narrow()},
			{"index", nodeIndex_},
			{"position", {position_.x, position_.y}},
			{"patrolTime", patrolTime},
			{"patrolRadius", patrolRadius}
			}
		};
	}

	void Deserialize(const nlohmann::json& i_json) override
	{
		nodeIndex_ = i_json["index"];
		position_ = { i_json["position"][0] , i_json["position"][1] };
		patrolTime = i_json["patrolTime"];
		patrolRadius = i_json["patrolRadius"];
		patrolTimeTe.text = Unicode::Widen(Common::doubleToString(patrolTime));
		patrolRadiusTe.text = Unicode::Widen(Common::doubleToString(patrolRadius));
	}

	void DebugDraw(Actor* pActor) override
	{
		Circle patrolRadiusCircle = { pActor->GetPosition(), patrolRadius };
		patrolRadiusCircle.draw({255,0,0,0.5});
		patrolRadiusCircle.drawFrame(1, Palette::Red);
	}
private:
	double patrolTime = 2.0;
	double patrolRadius = 96.0;
	TextEditState patrolTimeTe;
	TextEditState patrolRadiusTe;

};
