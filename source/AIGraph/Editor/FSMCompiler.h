#pragma once
#include "Core/Singleton.h"
#include "AIGraph/Editor/FSMGraph.h"
#include  "AIGraph/StateMachine/StateController.h"
class FSMCompiler : public Singleton<FSMCompiler>
{
public:
	FSMCompiler(){}
	~FSMCompiler() = default;

	/// @brief FSMグラフからステートコントローラをコンパイルする
	/// @param i_pGraph コンパイルするFSMグラフ
	/// @return コンパイルされたステートコントローラ
	StateController* CompileFromGraph(FSMGraph* i_pGraph);
	
private:
	FSMGraph* compilingFSMGraph_ = nullptr;
	StateController* outputStateController_ = nullptr;

};
