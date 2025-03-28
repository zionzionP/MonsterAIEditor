#include "AIGraph/Editor/FSMGraphSaveLoad.h"
#include "AIGraph/Editor/FSMGraph.h"
#include "Core/json.hpp"
#include <iostream>
#include <fstream>

#include "AIGraph/Editor/Node/Action/FSMGraphAttackActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphChaseActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphChaseAroundActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphPatrolActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphSetAroundIndexActionNode.h"
#include "AIGraph/Editor/Node/Action/FSMGraphWaitActionNode.h"
#include "AIGraph/Editor/Decision/FSMGraphActionDoneDecision.h"
#include "AIGraph/Editor/Decision/FSMGraphDistanceDecision.h"

#define LOAD_NODE(NodeType) \
    else if (nodeType == #NodeType) \
    { \
        auto spNode = std::make_shared<NodeType>(); \
        i_pGraph->AddNode(spNode); \
        spNode->Deserialize(i_json); \
    }
#define LOAD_DECISION(DecisionType) \
    else if (decisionType == #DecisionType) \
    { \
        auto spDecision = std::make_shared<DecisionType>(); \
        i_pGraph->AddDecision(spDecision);\
        spDecision->Deserialize(i_json);\
    }

void FSMGraphSaveLoad::SaveGraph(const FSMGraph* i_pGraph, const std::string& i_filePath)
{
	auto& nodes = i_pGraph->GetNodes();
	auto& edges = i_pGraph->GetEdges();
	auto& decisions = i_pGraph->GetDecisions();

	nlohmann::json jsonArrayToSave = nlohmann::json::array();
	for (auto& node : nodes)
	{
		jsonArrayToSave.push_back(node->Serialize());
	}
	for (auto& decision : decisions)
	{
		jsonArrayToSave.push_back(decision->Serialize());
	}
	for (auto& edge : edges)
	{
		jsonArrayToSave.push_back(edge->Serialize());
	}		
	std::ofstream ofs(i_filePath);
	if (!ofs) {
		Print << U"save failed";
		return;
	}

	ofs << jsonArrayToSave.dump(4) << std::endl;
	ofs.close();
	Print << U"save successful";
}


void FSMGraphSaveLoad::LoadGraph(FSMGraph* i_pGraph, const std::string& i_filePath)
{
	i_pGraph->ClearGraph();

	std::ifstream ifs(i_filePath);

	if (!ifs) {
		Print << U"load failed";
		return;
	}

	nlohmann::json jsonArrayToLoad = nlohmann::json::array();

	try {
		ifs >> jsonArrayToLoad;
	}
	catch (const nlohmann::json::parse_error& e) {
		Print << U"load failed";
		return;
	}
	ifs.close();

	for (const auto& element : jsonArrayToLoad)
	{
		if (element.is_array() && element.size() == 2)
		{
			auto type = element[0];
			const auto& data = element[1];

			if (type == U"node")
			{
				LoadNode(data, i_pGraph);
			}
			else if (type == U"decision")
			{
				LoadDecision(data, i_pGraph);
			}
			else if (type == U"edge")
			{
				LoadEdge(data, i_pGraph);
			}
			else {
				//Print << U"Unknown type: " << type;
				Print << U"load failed";
			}
		}
	}
	Print << U"load successful";
}

void FSMGraphSaveLoad::LoadNode(const nlohmann::json& i_json, FSMGraph* i_pGraph)
{
	std::string nodeType = i_json["typeName"].get<std::string>();
	if (!nodeType.empty())
	{
		if (false){}
		LOAD_NODE(FSMGraphWaitActionNode)
		LOAD_NODE(FSMGraphAttackActionNode)
		LOAD_NODE(FSMGraphChaseActionNode)
		LOAD_NODE(FSMGraphPatrolActionNode)
		LOAD_NODE(FSMGraphSetAroundIndexActionNode)
		LOAD_NODE(FSMGraphChaseAroundActionNode)
		else
		{
			Print << U"unknow type";
		}
	}
}

void FSMGraphSaveLoad::LoadDecision(const nlohmann::json& i_json, FSMGraph* i_pGraph)
{
	std::string decisionType = i_json["typeName"].get<std::string>();
	if (!decisionType.empty())
	{
		if (false) {}
		LOAD_DECISION(FSMGraphDistanceDecision)
		LOAD_DECISION(FSMGraphActionDoneDecision)
		else
		{
			Print << U"unknow type";
		}
	}
}

void FSMGraphSaveLoad::LoadEdge(const nlohmann::json& i_json, FSMGraph* i_pGraph)
{
	auto& nodes = i_pGraph->GetNodes();
	auto& decisions = i_pGraph->GetDecisions();
	int32 fromNodeIndex = i_json["fromNodeIndex"];
	int32 toNodeIndex = i_json["toNodeIndex"];	
	auto spEdge = std::make_shared<FSMGraphEdge>(nodes[fromNodeIndex].get(), nodes[toNodeIndex].get());
	
	int32 attachedDecisionIndex = i_json["attachedDecisionIndex"];
	spEdge->SetAttachedDecision(decisions[attachedDecisionIndex].get());

	int32 priority = i_json["priority"];
	spEdge->SetTransitionPriority(priority);
	if (priority != 0)
	{
		spEdge->GetProperty().SetPriorityTe(Format(priority));
	}

	decisions[attachedDecisionIndex].get()->setOwnerEdge(spEdge.get());
	decisions[attachedDecisionIndex].get()->SetIsAttached(true);

	i_pGraph->AddEdge(spEdge);
	spEdge->SetEdgeIndex(i_json["index"]);
}



