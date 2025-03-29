#pragma once
class FSMGraph;

enum class DecisionType
{
	FSMGraphDistanceDecision = 0,
	FSMGraphActionDoneDecision,

	FSMGraphDecisionMAX,
};

class FSMGraphAddDecisionPanel
{
public:
	FSMGraphAddDecisionPanel(FSMGraph* pOwner):
		pOwnerGraph_(pOwner)
	{
		SetupAddDecisionButton();
	};
	~FSMGraphAddDecisionPanel() = default;

	void AddDecision();
	void SetupAddDecisionButton();
	void DrawAddDecisionButton();

private:
	const Font font{ FontMethod::MSDF, 48 };
	FSMGraph* pOwnerGraph_ = nullptr;
	Rect addDecisionWindow_{ 585, 655, 1010, 240 };
	Vec2 panalTL = { 585,655 };
	std::vector<RectF> addDecisionButtons_;
};
