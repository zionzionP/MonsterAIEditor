#include "AIGraph/Editor/Decision/FSMGraphDistanceDecision.h"

void FSMGraphDistanceDecision::Update()
{
	super::Update();
	if (minDistanceTe.text.any())
	{
		minDistance = std::stod(minDistanceTe.text.toUTF8());
	}
	if (maxDistanceTe.text.any())
	{
		maxDistance = std::stod(maxDistanceTe.text.toUTF8());
	}
}

void FSMGraphDistanceDecision::Draw()
{
	super::Draw();
	displayNode_.draw(Palette::White);
	double x = displayNode_.tl().x;
	double y = displayNode_.tl().y;
	font(U"プレイヤーとの距離").draw(20, Vec2{ x + 5, y + 5 }, Palette::Black);
	font(U"Min").draw(25, Vec2{ x + 5, y + 40 }, Palette::Black);
	font(U"Max").draw(25, Vec2{ x + 5, y + 80 }, Palette::Black);
	SimpleGUI::TextBox(minDistanceTe, Vec2{ x + 75, y + 40 }, 50);
	SimpleGUI::TextBox(maxDistanceTe, Vec2{ x + 75, y + 80 }, 50);
}

void FSMGraphDistanceDecision::DebugDraw(Actor* pActor)
{
	Circle minCircle = { pActor->GetPosition(), minDistance };
	Circle maxCircle = { pActor->GetPosition(), maxDistance };
	maxCircle.drawFrame(maxDistance-minDistance,0, { 255,0,0,0.5 });
	minCircle.drawFrame(1, Palette::Blue);
	maxCircle.drawFrame(1, Palette::Red);
}


nlohmann::json FSMGraphDistanceDecision::Serialize() const
{
	return { "decision",{
		{"typeName", typeName_.narrow()},
		{"index", decisionIndex_},
		{"position", {position_.x, position_.y}},
		{"minDistance", minDistance},
		{"maxDistance", maxDistance}
		}
	};
}

void FSMGraphDistanceDecision::Deserialize(const nlohmann::json& i_json)
{
	decisionIndex_ = i_json["index"];
	position_ = { i_json["position"][0] , i_json["position"][1] };
	minDistance = i_json["minDistance"];
	maxDistance = i_json["maxDistance"];
	minDistanceTe.text = Unicode::Widen(Common::doubleToString(minDistance));
	maxDistanceTe.text = Unicode::Widen(Common::doubleToString(maxDistance));
}

