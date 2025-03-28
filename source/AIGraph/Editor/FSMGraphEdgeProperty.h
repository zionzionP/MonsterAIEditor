#pragma once
#include "AIGraph/Editor/FSMGraphProperty.h"
class FSMGraphEdge;

class FSMGraphEdgeProperty : public FSMGraphProperty
{
public:
	FSMGraphEdgeProperty(FSMGraphEdge* owner):
		ownerEdge_(owner)
	{};
	~FSMGraphEdgeProperty()override = default;	

	void Update() override;
	void Draw() override;	

	const Vec2& GetSlotPos()const { return decisionSlot_.center(); }
	bool IsSlotAttached(const RectF& decision)const { return decisionSlot_.intersects(decision); }
	void SetPriorityTe(const String& i_string) { priorityTe.text = i_string; }

private:
	friend FSMGraphEdge;
	FSMGraphEdge* ownerEdge_;
	//UI
	const Font font{ FontMethod::MSDF, 48 };
	RectF decisionSlot_{ Vec2{1345,270}, 200, 120 };
	TextEditState priorityTe;
	
};
