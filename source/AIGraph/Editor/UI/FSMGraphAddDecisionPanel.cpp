#include "FSMGraphAddDecisionPanel.h"

#include "AIGraph/Editor/FSMGraph.h"
#include "AIGraph/Editor/Decision/FSMGraphActionDoneDecision.h"
#include "AIGraph/Editor/Decision/FSMGraphDistanceDecision.h"

void FSMGraphAddDecisionPanel::AddDecision()
{
	if (addDecisionWindow_.leftClicked())
	{
		for (int32 i = 0; i < static_cast<int32>(DecisionType::FSMGraphDecisionMAX); ++i)
		{
			if (addDecisionButtons_[i].leftClicked())
			{
				switch (static_cast<DecisionType>(i))
				{
				case DecisionType::FSMGraphDistanceDecision:
				{
					auto spDecision = std::make_shared<FSMGraphDistanceDecision>();
					spDecision.get()->SetPosition({ Cursor::PosF() });
					pOwnerGraph_->AddDecision(spDecision);
					pOwnerGraph_->SetDraggingDecision(spDecision.get());
				}
				break;
				case DecisionType::FSMGraphActionDoneDecision:
				{
					auto spDecision = std::make_shared<FSMGraphActionDoneDecision>();
					spDecision.get()->SetPosition({ Cursor::PosF() });
					pOwnerGraph_->AddDecision(spDecision);
					pOwnerGraph_->SetDraggingDecision(spDecision.get());
				}
				break;
				default:
					break;
				}
			}
		}
	}
}
void FSMGraphAddDecisionPanel::SetupAddDecisionButton()
{
	for (int32 i = 0; i < static_cast<int32>(DecisionType::FSMGraphDecisionMAX); ++i)
	{
		Vec2 buttonTL = panalTL + Vec2{ 5, 5 } + i * Vec2{210, 0}; ;
		addDecisionButtons_.emplace_back(RectF{ buttonTL.x,buttonTL.y,200,120 });
	}
}
void FSMGraphAddDecisionPanel::DrawAddDecisionButton()
{
	addDecisionWindow_.draw(ColorF{ 1.0, 1.0, 1.0, 0.5 });
	addDecisionButtons_[0].draw(Palette::White);
	font(U"プレイヤーとの距離").draw(20, addDecisionButtons_[0].tl(), Palette::Black);
	addDecisionButtons_[1].draw(Palette::White);
	font(U"現在の行動が完了").draw(20, addDecisionButtons_[1].tl(), Palette::Black);
}
