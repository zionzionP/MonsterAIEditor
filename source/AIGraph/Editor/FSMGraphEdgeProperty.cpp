#include "AIGraph/Editor/FSMGraphEdgeProperty.h"
#include "AIGraph/Editor/FSMGraphEdge.h"

void FSMGraphEdgeProperty::Update()
{
	if (priorityTe.textChanged)
	{
		if (priorityTe.text.any())
		{
			ownerEdge_->SetTransitionPriority(std::stoi(priorityTe.text.narrow()));
		}
	}
}

void FSMGraphEdgeProperty::Draw()
{
	int32 fromIndex = ownerEdge_->GetFromNode()->GetNodeIndex();
	int32 toIndex = ownerEdge_->GetToNode()->GetNodeIndex();

	font(U"遷移").draw(30, Vec2{ 1345, 100 }, Palette::Black);	
	font(U"From").draw(20, Vec2{ 1345, 140 }, Palette::Black);	
	font(U"Node").draw(20, Vec2{ 1405, 140 }, Palette::Black);
	font(fromIndex).draw(20, Vec2{ 1465, 140 }, Palette::Black);
	font(U"To").draw(20, Vec2{ 1345, 170 }, Palette::Black);
	font(U"Node").draw(20, Vec2{ 1405, 170 }, Palette::Black);
	font(toIndex).draw(20, Vec2{ 1465, 170 }, Palette::Black);

	font(U"遷移条件").draw(30, Vec2{ 1345, 230 }, Palette::Black);
	decisionSlot_.draw(ColorF(1,1,1,0.5));


	font(U"優先度").draw(20, Vec2{ 1475, 615 }, Palette::Black);
	SimpleGUI::TextBox(priorityTe, Vec2{ 1535, 610 }, 50);
}
