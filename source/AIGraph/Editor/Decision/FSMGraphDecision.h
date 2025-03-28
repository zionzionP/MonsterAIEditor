#pragma once
#include "AIGraph/Editor/FSMGraphEdge.h"
#include "AIGraph/Editor/FSMGraphObject.h"

class FSMGraphDecision : public FSMGraphObject
{
public:
	FSMGraphDecision() {};
	~FSMGraphDecision()override = default;

public:

	void Update() override;
	void Draw() override;
	bool IsClicked() const { return displayNode_.leftClicked(); }
	bool IsReleased() const { return displayNode_.leftReleased(); }

public:
	const RectF& GetDisplayNode() const { return displayNode_; }

	const Vec2& GetPosition() const { return position_; }
	void SetPosition(const Vec2& i_pos) { position_ = i_pos; }

	bool GetIsDragging() const { return isDragging_; }
	void SetIsDragging(bool in) { isDragging_ = in; }

	bool GetIsAttached() const { return isAttached_; }
	void SetIsAttached(bool in) { isAttached_ = in; }

	bool GetIsActive() const { return isActive_; }
	void SetIsActive(bool in) { isActive_ = in; }

	int32 GetDecisionIndex() const { return decisionIndex_; }
	void SetDecisionIndex(int32 i_index) { decisionIndex_ = i_index; }

	FSMGraphEdge* getOwnerEdge() const { return ownerEdge_; }
	void setOwnerEdge(FSMGraphEdge* pEdge) { ownerEdge_ = pEdge; }


	void RefreshPosition() { displayNode_.setCenter(position_); }
	void CheckIsAttached();	
	void CheckIsActive();

	virtual void RegisterDecision(StateController* io_controller) {}
	virtual nlohmann::json Serialize() const { return nlohmann::json(); }
	virtual void Deserialize(const nlohmann::json& i_json) {}

	virtual void DebugDraw(Actor* pActor) {}

protected:
	FSMGraphEdge* ownerEdge_ = nullptr;
	const Font font{ FontMethod::MSDF, 48 };
	String typeName_ = U"";
	RectF displayNode_{ Arg::center(0,0), 200, 120 };
	Vec2 position_ = { 0,0 };
	int32 decisionIndex_ = -1;
	bool isDragging_ = false;
	bool isAttached_ = false;
	bool isActive_ = false;
};
