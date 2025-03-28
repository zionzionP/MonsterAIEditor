#include "AIGraph/Editor/Decision/FSMGraphActionDoneDecision.h"

void FSMGraphActionDoneDecision::Update()
{
	super::Update();
}

void FSMGraphActionDoneDecision::Draw()
{
	super::Draw();
	displayNode_.draw(Palette::White);
	double x = displayNode_.tl().x;
	double y = displayNode_.tl().y;
	font(U"現在の行動が完了").draw(20, Vec2{ x + 5, y + 5 }, Palette::Black);
}

nlohmann::json FSMGraphActionDoneDecision::Serialize() const
{
	return { "decision",{
		{"typeName", typeName_.narrow()},
		{"index", decisionIndex_},
		{"position", {position_.x, position_.y}}
		}
	};
}

void FSMGraphActionDoneDecision::Deserialize(const nlohmann::json& i_json)
{
	decisionIndex_ = i_json["index"];
	position_ = { i_json["position"][0] , i_json["position"][1] };
}
