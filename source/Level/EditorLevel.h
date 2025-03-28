#pragma once
#include "Level/Level.h"
#include "Level/LevelManager.h"
#include "AIGraph/Editor/FSMGraph.h"
#include "AIGraph/Editor/Node/FSMGraphNode.h"

//AIグラフエディターレベル
class EditorLevel : public Level
{
public:
	EditorLevel() {}
	virtual ~EditorLevel() = default;

	void Start() override;
	void Update() override;
	void Exit() override;

private:
	FSMGraph* createNewFsmGraph();

private:
	RenderTexture renderTexture{ 1600, 900 };
	Font font{ FontMethod::MSDF, 48 };

	FSMGraph* currentFSMGraph_ = nullptr;

	bool isAddingEdge = false;


};
