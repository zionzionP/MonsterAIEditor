#include "AIGraph/Editor/FSMGraph.h"
#include "AIGraph/Editor/UI/FSMGraphSelectEnemyPanel.h"
void FSMGrapSelectEnemyPanel::Update()
{
	handleEvents();
}

void FSMGrapSelectEnemyPanel::Draw()
{
	drawSelectEnemyButton();
	SimpleGUI::Button(U"OK!", Vec2{ 500, 860 });
}

void FSMGrapSelectEnemyPanel::handleEvents()
{
	selectEnemy();
	geneEnemyWithoutAI();
}

void FSMGrapSelectEnemyPanel::selectEnemy()
{
	if (selectEnemyPanel_.leftClicked() && !SimpleGUI::Button(U"OK!", Vec2{ 500, 860 }))
	{
		bool isClicked = false;
		for (int32 i = 0; i < static_cast<int32>(EnemyType::EnemyTypeMAX); ++i)
		{			
			if (selectEnemyButtons_[i].IsClicked())
			{
				isClicked = true;
				selectEnemyButtons_[i].SetIsSelected(true);
				selectedEnemyIndex_ = i;
			}
			else
			{
				selectEnemyButtons_[i].SetIsSelected(false);
			}
		}
		if (!isClicked)
		{
			selectedEnemyIndex_ = -1;
		}
	}	
}

void FSMGrapSelectEnemyPanel::geneEnemyWithoutAI()
{
	if(SimpleGUI::Button(U"OK!", Vec2{ 500, 860 }))
	{
		pOwnerGraph_->GetSetup().SetupEditorLevelDebugArea();
		switch (selectedEnemyIndex_)
		{
		case static_cast<int32>(EnemyType::Ghost):
		{
			pOwnerGraph_->GetSetup().SetupEditorLevelDebugArea();
			pOwnerGraph_->SetCurrentEnemy(pOwnerGraph_->GetSetup().GenerateGhostWithoutAI());
		}
		break;
		case static_cast<int32>(EnemyType::SkeletonKnight):
		{
			pOwnerGraph_->GetSetup().SetupEditorLevelDebugArea();
			pOwnerGraph_->SetCurrentEnemy(pOwnerGraph_->GetSetup().GenerateSkeletonWithoutAI());
		}
		break;
		default:
			break;
		}
	}

}

void FSMGrapSelectEnemyPanel::setupSelectEnemyButton()
{
	for (int32 i = 0; i < static_cast<int32>(EnemyType::EnemyTypeMAX); ++i)
	{
		Vec2 pos = panalTL + Vec2{ 5, 5 } + Vec2{0.5 * buttonSize, 0.5 * buttonSize} + i * Vec2{ buttonSize, 0 };
		selectEnemyButtons_.emplace_back(pos);
		selectEnemyButtons_[i].SetTexture(texturePaths_[i]);
	}
}

void FSMGrapSelectEnemyPanel::drawSelectEnemyButton()
{
	selectEnemyPanel_.draw(ColorF{ 1.0, 1.0, 1.0, 0.5 });
	const uint64 millisecTime = Time::GetMillisec();
	for (auto& button : selectEnemyButtons_)
	{
		button.Draw(millisecTime);
	}
}
