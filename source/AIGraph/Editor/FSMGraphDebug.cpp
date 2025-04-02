#include "AIGraph/Editor/FSMGraphDebug.h"

#include "AIGraph/Editor/FSMGraph.h"

void FSMGraphDebug::Update()
{
	SimpleGUI::CheckBox(isDebugDrawNavmesh_, U"Navmesh", Vec2{ 5, 55 });
	SimpleGUI::CheckBox(isDebugDrawPath_, U"Path", Vec2{ 145, 55 });
	SimpleGUI::CheckBox(isDebugDrawDistanceRange_, U"Range", Vec2{ 240,55 });
	SimpleGUI::CheckBox(isDebugDrawActiveNode_, U"ActiveNode", Vec2{350,55 });	
}

void FSMGraphDebug::DebugDraw()
{
	DebugDrawNavmesh();
	DebugDrawPath();
	DebugDrawDistanceRange();
	DebugDrawActiveNode();
}

void FSMGraphDebug::DebugDrawNavmesh()
{
	if (isDebugDrawNavmesh_)
	{
		pOwnerGraph_->setup.GetNavMesh().RenderMesh();
	}
}
void FSMGraphDebug::DebugDrawPath()
{
	if (isDebugDrawPath_)
	{
		if (pOwnerGraph_->pCurrentEnemy_ != nullptr)
		{
			pOwnerGraph_->pCurrentEnemy_->GetAgent().ShowPath();
		}		
	}	
}
void FSMGraphDebug::DebugDrawDistanceRange()
{
	if (isDebugDrawDistanceRange_ && pOwnerGraph_->pCurrentEnemy_ != nullptr)
	{
		if (pOwnerGraph_->pSelectedNode_ )
		{
			pOwnerGraph_->pSelectedNode_->DebugDraw(pOwnerGraph_->pCurrentEnemy_);
		}
		if (pOwnerGraph_->pSelectedEdge_)
		{
			pOwnerGraph_->pSelectedEdge_->DebugDraw(pOwnerGraph_->pCurrentEnemy_);
		}
	}
}
void FSMGraphDebug::DebugDrawActiveNode()
{
	if (isDebugDrawActiveNode_)
	{
		if (pOwnerGraph_->pCurrentEnemy_ != nullptr)
		{
			StateController* pController = pOwnerGraph_->pCurrentEnemy_->GetStateController();
			if (!pController)
			{
				return;
			}
			auto& stateMap = pController->GetStateMap();
			auto* pCurrentState = pController->GetCurrentState();
			int32 currentStateIndex = -1;
			for (auto& pair : stateMap)
			{
				if (pair.second.get() == pCurrentState)
				{
					currentStateIndex = pair.first;
					break;
				}
			}
			pOwnerGraph_->nodes_[currentStateIndex]->DrawFrame();
		}
	}	
}
