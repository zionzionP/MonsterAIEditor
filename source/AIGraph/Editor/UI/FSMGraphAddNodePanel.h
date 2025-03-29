#pragma once

class FSMGraph;

enum class NodeType
{
	FSMGraphWaitActionNode = 0,
	FSMGraphAttackActionNode,
	FSMGraphChaseActionNode,
	FSMGraphPatrolActionNode,
	FSMGraphSetAroundIndexActionNode,
	FSMGraphChaseAroundActionNode,

	FSMGraphActionNodeMAX,
};

class FSMGraphAddNodePanel
{
public:
	FSMGraphAddNodePanel(FSMGraph* pOwner) :
		pOwnerGraph_(pOwner)
	{
		SetupAddNodeButton();
	}
	~FSMGraphAddNodePanel() = default;

	void AddNode();
	void SetupAddNodeButton();
	void DrawAddNodeButton();

private:
	const Font font{ FontMethod::MSDF, 48 };
	FSMGraph* pOwnerGraph_ = nullptr;
	Rect addNodeWindow_{ 585, 655, 1010, 240 };
	Vec2 panalTL = { 585,655 };
	std::vector<RectF> addNodeButtons_;

};
