#pragma once
#include "AIGraph/Editor/FSMGraphObject.h"
#include "AIGraph/StateMachine/StateController.h"
#include "Core/json.hpp"

class FSMGraphEdge;

class FSMGraphNode : public FSMGraphObject
{
public:
	FSMGraphNode() {};
	~FSMGraphNode()override = default;

public:
	
	void Update() override;	
	void Draw() override;
	bool IsClicked() const { return displayNode_.leftClicked(); }
	bool IsReleased() const { return displayNode_.leftReleased(); }

public:
	const RectF& GetDisplayNode() const { return displayNode_; }

	const Vec2& GetPosition() const { return position_; }
	void SetPosition(const Vec2& i_pos) { position_ = i_pos; }
	void RefreshPosition(){ displayNode_.setCenter(position_); }

	int32 GetNodeIndex() const {return nodeIndex_;}
	void SetNodeIndex(int32 index) {nodeIndex_ = index;}

	bool GetIsDragging() const { return isDragging_; }
	void SetIsDragging(bool in) { isDragging_ = in; }

public:
	virtual void RegisterAction(StateController* io_controller) {}
	virtual nlohmann::json Serialize() const { return nlohmann::json(); }
	virtual void Deserialize(const nlohmann::json& i_json) {}

	virtual void UpdateProperty() {}
	virtual void DrawProperty();
	virtual void DebugDraw(Actor* pActor){}
	void DrawFrame()const { displayNode_.drawFrame(0,5, Palette::Yellow); }

private:
	void drawDisplayNameInNode();
	

protected:
	String typeName_ = U"";
	String displayName_ = U"";
	Color color_ = Palette::White;
	RectF displayNode_{ Arg::center(0,0), 100, 60 };
	const Font font{ FontMethod::MSDF, 30 };

	Vec2 position_ = { 0,0 };
	int32 nodeIndex_ = -1;
	bool isDragging_ = false;

	std::vector<FSMGraphEdge*> outEdges_;
	//std::vector<FSMGraphEdge*> inEdges_;

};
