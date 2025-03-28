#pragma once
#include "AIGraph/Editor/Node/FSMGraphNode.h"

class FSMGraphActionNode : public FSMGraphNode
{
public:
	FSMGraphActionNode() {};
	~FSMGraphActionNode() override = default;

	virtual void RegisterAction(StateController* io_controller){}
	
};
