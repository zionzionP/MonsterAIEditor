#pragma once
#include "Core/json.hpp"
#include "Core/Singleton.h"

class FSMGraph;

class FSMGraphSaveLoad : public Singleton<FSMGraphSaveLoad>
{
public:
	FSMGraphSaveLoad() {};
	~FSMGraphSaveLoad() = default;

	void SaveGraph(const FSMGraph* i_pGraph, const std::string& i_filePath);
	void LoadGraph(FSMGraph* i_pGraph, const std::string& i_filePath);

	void LoadNode(const nlohmann::json& i_json, FSMGraph* i_pGraph);
	void LoadDecision(const nlohmann::json& i_json, FSMGraph* i_pGraph);
	void LoadEdge(const nlohmann::json& i_json, FSMGraph* i_pGraph);

};
