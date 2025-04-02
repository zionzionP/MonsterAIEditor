#include "AIGraph/Editor/UI/FSMGraphAddNodePanel.h"

#include "AIGraph/Editor/FSMGraph.h"
#include "AIGraph/Editor/Node/Action/FSMGraphAttackActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphChaseActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphChaseAroundActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphPatrolActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphSetAroundIndexActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphWaitActionNode.h"
#define ADD_NODE(NodeTypeName) \
	case NodeType::NodeTypeName: \
	{ \
		auto spNode = std::make_shared<NodeTypeName>(); \
		spNode.get()->SetPosition({ Cursor::PosF() }); \
		pOwnerGraph_->AddNode(spNode); \
		spNode.get()->SetIsDragging(true); \
	} \
	break; \

void FSMGraphAddNodePanel::AddNode()
{
	if (addNodeWindow_.leftClicked())
	{
		for (int32 i = 0; i < static_cast<int32>(NodeType::FSMGraphActionNodeMAX); ++i)
		{
			if (addNodeButtons_[i].leftClicked())
			{
				switch (static_cast<NodeType>(i))
				{
					ADD_NODE(FSMGraphWaitActionNode);
					ADD_NODE(FSMGraphAttackActionNode);
					ADD_NODE(FSMGraphChaseActionNode);
					ADD_NODE(FSMGraphPatrolActionNode);
					ADD_NODE(FSMGraphSetAroundIndexActionNode);
					ADD_NODE(FSMGraphChaseAroundActionNode);
				default:
					break;
				}
			}
		}
	}
}

void FSMGraphAddNodePanel::SetupAddNodeButton()
{
	for (int32 i = 0; i < static_cast<int32>(NodeType::FSMGraphActionNodeMAX); ++i)
	{
		Vec2 buttonTL = panalTL + Vec2{ 5,5 } + i * Vec2{110,0};		;
		addNodeButtons_.emplace_back(RectF{ buttonTL.x,buttonTL.y,100,60 });
	}
}

void FSMGraphAddNodePanel::DrawAddNodeButton()
{
	addNodeWindow_.draw(ColorF{ 1.0, 1.0, 1.0, 0.5 });

	addNodeButtons_[0].draw(Palette::Purple);
	font(U"Wait").draw(25, addNodeButtons_[0].tl(), Palette::White);
	addNodeButtons_[1].draw(Palette::Red);
	font(U"Attack").draw(25, addNodeButtons_[1].tl(), Palette::White);
	addNodeButtons_[2].draw(Palette::Green);
	font(U"Chase").draw(25, addNodeButtons_[2].tl(), Palette::White);
	addNodeButtons_[3].draw(Palette::Green);
	font(U"Patrol").draw(25, addNodeButtons_[3].tl(), Palette::White);
	addNodeButtons_[4].draw(Palette::Blue);
	font(U"Set").draw(25, addNodeButtons_[4].tl(), Palette::White);
	font(U"Point").draw(25, addNodeButtons_[4].tl() + Vec2{0,25}, Palette::White);
	addNodeButtons_[5].draw(Palette::Green);
	font(U"Chase").draw(25, addNodeButtons_[5].tl(), Palette::White);
	font(U"Around").draw(25, addNodeButtons_[5].tl() + Vec2{0, 25}, Palette::White);
}
