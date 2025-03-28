#include "AIGraph/Editor/FSMCompiler.h"
#include "AIGraph/StateMachine/AIFactory.h"

StateController* FSMCompiler::CompileFromGraph(FSMGraph* i_pGraph)
{
	compilingFSMGraph_ = i_pGraph;
	StateController* pController = DBG_NEW StateController();

	auto& nodes = compilingFSMGraph_->GetNodes();
	auto& edges = compilingFSMGraph_->GetEdges();

	AIFactory* pFactory = AIFactory::Get();
	for (int32 index = 0; index < nodes.size(); ++index)
	{
		//ステートの登録
		pFactory->RegisterState(index, pController);
		//アクションの登録
		nodes[index].get()->RegisterAction(pController);
		//アクションをステートにセット
		pController->GetStateMap()[index]->GetActions().push_back(pController->GetActionMap()[index].get());
	}

	for (int32 index = 0; index < edges.size(); ++index)
	{
		//遷移条件の登録
		edges[index]->GetAttachedDecision()->RegisterDecision(pController);
		int32 fromStateIndex = edges[index]->GetFromNode()->GetNodeIndex();
		int32 toStateIndex = edges[index]->GetToNode()->GetNodeIndex();
		int32 priority = edges[index]->GetTransitionPriority();
		//遷移条件がtrueの場合、遷移先(TrueState)に遷移
		FSM::Transition transition(pController->GetDecisionMap()[index].get(), pController->GetStateMap()[toStateIndex].get(), pController->GetStateMap()[fromStateIndex].get(), priority);
		//fromのステートに遷移をセット
		pController->GetStateMap()[fromStateIndex].get()->GetTransitions().push_back(transition);
	}
	//優先度からの遷移のソート
	for (auto& state : pController->GetStateMap())
	{
		auto& transisions = state.second.get()->GetTransitions();
		if (transisions.size() > 1)
		{
			std::sort(transisions.begin(), transisions.end(), [](FSM::Transition t1, FSM::Transition t2) {return t1.priority > t2.priority; });
		}
	}

	pController->SetCurrentState(pController->GetStateMap()[0].get());
	return pController;
}
