#include "AIGraph/Editor/Decision/FSMGraphDecision.h"
void FSMGraphDecision::Update()
{
	RefreshPosition();
	CheckIsAttached();
	CheckIsActive();
}
void FSMGraphDecision::Draw()
{
	if (isActive_ && isSelected_)
	{
		displayNode_.drawFrame(5.0, Palette::Yellow);
	}
}

void FSMGraphDecision::CheckIsAttached()
{
	if (ownerEdge_)
	{
		if (ownerEdge_->GetProperty().GetSlotPos() != position_)
		{
			SetIsAttached(false);
			ownerEdge_->SetAttachedDecision(nullptr);
		}
	}
}

void FSMGraphDecision::CheckIsActive()
{
	if (ownerEdge_)
	{
		isActive_ = ownerEdge_->GetIsSelected();
	}
	else
	{
		isActive_ = true;
	}

}
