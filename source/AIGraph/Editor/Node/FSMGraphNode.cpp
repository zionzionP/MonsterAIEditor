#include "AIGraph/Editor/Node/FSMGraphNode.h"

void FSMGraphNode::Update()
{
	RefreshPosition();
}

void FSMGraphNode::Draw()
{
	displayNode_.draw(color_);
	if (isSelected_)
	{		
		DrawFrame();
		UpdateProperty();
		DrawProperty();
	}
	drawDisplayNameInNode();
}

void FSMGraphNode::drawDisplayNameInNode()
{
	double fontSize = 25;
	if (displayNode_.w < displayName_.size() * 0.5 * fontSize)
	{
		fontSize = displayNode_.w / (displayName_.size() * 0.5);
		font(displayName_).draw(fontSize, displayNode_.tl(), Palette::White);
		return;
	}
	font(displayName_).draw(fontSize, displayNode_.tl(), Palette::White);
}

void FSMGraphNode::DrawProperty()
{
	font(displayName_).draw(30, Vec2{ 1345, 100 }, Palette::Black);
	for (int i = 0; i < descriptionForDisplay_.size(); ++i)
	{
		font(descriptionForDisplay_[i]).draw(20, Vec2{ 1345,135 } + i * Vec2{0,20}, Palette::Black);
	}
}

void FSMGraphNode::splitDescription()
{
	int wordNum = 12;
	for (int i = 0; i < description_.length(); i += wordNum)
	{
		descriptionForDisplay_.push_back(description_.substr(i, wordNum));
	}
}

