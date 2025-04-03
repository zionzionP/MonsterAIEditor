#include "AIGraph/StateMachine/AIFactory.h"

#include "AIGraph/Editor/FSMCompiler.h"
#include "AIGraph/Editor/FSMGraphSaveLoad.h"
#include "AIGraph/StateMachine/State.h"
#include "AIGraph/StateMachine/Transition.h"
#include "AIGraph/StateMachine/Action/Action.h"
#include "AIGraph/StateMachine/Decision/Decision.h"
#include "AIGraph/StateMachine/Decision/DistanceDecision.h"
#include "AIGraph/StateMachine/Action/PatrolAction.h"
#include "AIGraph/StateMachine/Action/ChaseAction.h"
#include "AIGraph/StateMachine/Action/AttackAction.h"
#include "AIGraph/StateMachine/Action/ChaseAroundAction.h"
#include "AIGraph/StateMachine/Decision/DistanceAndObstacleDecision.h"
#include "AIGraph/StateMachine/Action/SetAroundIndexAction.h"
#include "AIGraph/StateMachine/Action/PatrolInAreaAction.h"
#include "AIGraph/StateMachine/Decision/DistanceToPatrolPosDecision.h"
#include "AIGraph/StateMachine/Action/PatrolInRectAreaAction.h"
#include "AIGraph/StateMachine/Decision/PlayerInAreaDecision.h"

AIFactory::~AIFactory()
{
	delete pFsmGraphForLoad_;
}


void AIFactory::RegisterState(const int32& i_index, StateController* io_pController)
{
	io_pController->GetStateMap().emplace(i_index, DBG_NEW State);
}

StateController* AIFactory::SetupStateMachine()
{
	StateController* pController = DBG_NEW StateController();
	//ステートの設定
	pController->GetStateMap().emplace(StateId::PATROL_STATE, DBG_NEW State);
	pController->GetStateMap().emplace(StateId::CHASE_STATE, DBG_NEW State);
	pController->GetStateMap().emplace(StateId::ATTACK_STATE, DBG_NEW State);
	//アクションの設定
	pController->GetActionMap().emplace(ActionId::PATROL_ACTION, DBG_NEW PatrolAction);
	pController->GetActionMap().emplace(ActionId::CHASE_ACTION, DBG_NEW ChaseAction);
	pController->GetActionMap().emplace(ActionId::ATTACK_ACTION, DBG_NEW AttackAction);
	//遷移と遷移条件の設定
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_160_INF, DBG_NEW DistanceDecision(160.0, INFINITY));
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_80_160, DBG_NEW DistanceDecision(80.0, 160.0));
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_0_80, DBG_NEW DistanceAndObstacleDecision(0.0, 80));

	FSM::Transition transitionPtoC(pController->GetDecisionMap()[DISTANCE_DECISION_80_160].get(), pController->GetStateMap()[CHASE_STATE].get(), pController->GetStateMap()[PATROL_STATE].get());
	FSM::Transition transitionCtoP(pController->GetDecisionMap()[DISTANCE_DECISION_160_INF].get(), pController->GetStateMap()[PATROL_STATE].get(), pController->GetStateMap()[CHASE_STATE].get());
	FSM::Transition transitionCtoA(pController->GetDecisionMap()[DISTANCE_DECISION_0_80].get(), pController->GetStateMap()[ATTACK_STATE].get(), pController->GetStateMap()[CHASE_STATE].get());
	FSM::Transition transitionAtoC(pController->GetDecisionMap()[DISTANCE_DECISION_80_160].get(), pController->GetStateMap()[CHASE_STATE].get(), pController->GetStateMap()[ATTACK_STATE].get());

	//パトロールステート
	pController->GetStateMap()[PATROL_STATE].get()->GetActions().push_back(pController->GetActionMap()[PATROL_ACTION].get());
	pController->GetStateMap()[PATROL_STATE].get()->GetTransitions().push_back(transitionPtoC);
	//追跡ステート
	pController->GetStateMap()[CHASE_STATE].get()->GetActions().push_back(pController->GetActionMap()[CHASE_ACTION].get());
	pController->GetStateMap()[CHASE_STATE].get()->GetTransitions().push_back(transitionCtoP);
	pController->GetStateMap()[CHASE_STATE].get()->GetTransitions().push_back(transitionCtoA);
	//攻撃ステート
	pController->GetStateMap()[ATTACK_STATE].get()->GetActions().push_back(pController->GetActionMap()[ATTACK_ACTION].get());
	pController->GetStateMap()[ATTACK_STATE].get()->GetTransitions().push_back(transitionAtoC);


	pController->SetCurrentState(pController->GetStateMap()[PATROL_STATE].get());
	return pController;
}

StateController* AIFactory::SetupStateMachineType2()
{
	StateController* pController = DBG_NEW StateController();
	//ステートの設定
	pController->GetStateMap().emplace(StateId::PATROL_STATE, DBG_NEW State);
	pController->GetStateMap().emplace(StateId::CHASE_STATE, DBG_NEW State);
	pController->GetStateMap().emplace(StateId::ATTACK_STATE, DBG_NEW State);
	//アクションの設定
	pController->GetActionMap().emplace(ActionId::PATROL_ACTION, DBG_NEW PatrolAction);
	pController->GetActionMap().emplace(ActionId::CHASE_AROUND_ACTION, DBG_NEW ChaseAroundAction);
	pController->GetActionMap().emplace(ActionId::SET_AROUND_INDEX_ACTION, DBG_NEW SetAroundIndexAction);
	pController->GetActionMap().emplace(ActionId::ATTACK_ACTION, DBG_NEW AttackAction);
	//遷移と遷移条件の設定
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_160_INF, DBG_NEW DistanceDecision(160.0, INFINITY));
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_80_160, DBG_NEW DistanceDecision(80.0, 160.0));
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_0_80, DBG_NEW DistanceAndObstacleDecision(0.0, 80));

	FSM::Transition transitionPtoC(pController->GetDecisionMap()[DISTANCE_DECISION_80_160].get(), pController->GetStateMap()[CHASE_STATE].get(), pController->GetStateMap()[PATROL_STATE].get());
	FSM::Transition transitionCtoP(pController->GetDecisionMap()[DISTANCE_DECISION_160_INF].get(), pController->GetStateMap()[PATROL_STATE].get(), pController->GetStateMap()[CHASE_STATE].get());
	FSM::Transition transitionCtoA(pController->GetDecisionMap()[DISTANCE_DECISION_0_80].get(), pController->GetStateMap()[ATTACK_STATE].get(), pController->GetStateMap()[CHASE_STATE].get());
	FSM::Transition transitionAtoC(pController->GetDecisionMap()[DISTANCE_DECISION_80_160].get(), pController->GetStateMap()[CHASE_STATE].get(), pController->GetStateMap()[ATTACK_STATE].get());

	//パトロールステート
	pController->GetStateMap()[PATROL_STATE].get()->GetActions().push_back(pController->GetActionMap()[PATROL_ACTION].get());
	pController->GetStateMap()[PATROL_STATE].get()->GetTransitions().push_back(transitionPtoC);
	//追跡ステート
	pController->GetStateMap()[CHASE_STATE].get()->GetEnterActions().push_back(pController->GetActionMap()[SET_AROUND_INDEX_ACTION].get());
	pController->GetStateMap()[CHASE_STATE].get()->GetActions().push_back(pController->GetActionMap()[CHASE_AROUND_ACTION].get());
	pController->GetStateMap()[CHASE_STATE].get()->GetTransitions().push_back(transitionCtoP);
	pController->GetStateMap()[CHASE_STATE].get()->GetTransitions().push_back(transitionCtoA);
	//攻撃ステート
	pController->GetStateMap()[ATTACK_STATE].get()->GetActions().push_back(pController->GetActionMap()[ATTACK_ACTION].get());
	pController->GetStateMap()[ATTACK_STATE].get()->GetTransitions().push_back(transitionAtoC);


	pController->SetCurrentState(pController->GetStateMap()[PATROL_STATE].get());
	return pController;
}

StateController* AIFactory::SetupStateMachineSkeletonKnight()
{
	StateController* pController = DBG_NEW StateController();
	//ステートの設定
	pController->GetStateMap().emplace(StateId::PATROL_STATE, DBG_NEW State);
	pController->GetStateMap().emplace(StateId::CHASE_STATE, DBG_NEW State);
	pController->GetStateMap().emplace(StateId::ATTACK_STATE, DBG_NEW State);
	//アクションの設定		
	pController->GetActionMap().emplace(ActionId::PATROL_IN_RECT_AREA_ACTION, DBG_NEW PatrolInRectAreaAction(2.0, { 143,161.6 }, { 401,382.4 }));
	pController->GetActionMap().emplace(ActionId::PATROL_IN_RECT_AREA_ONCE_ACTION, DBG_NEW PatrolInRectAreaAction(0.0, { 143,161.6 }, { 401,382.4 }));
	pController->GetActionMap().emplace(ActionId::CHASE_AROUND_ACTION, DBG_NEW ChaseAroundAction);
	pController->GetActionMap().emplace(ActionId::SET_AROUND_INDEX_ACTION, DBG_NEW SetAroundIndexAction);
	pController->GetActionMap().emplace(ActionId::ATTACK_ACTION, DBG_NEW AttackAction);
	//遷移と遷移条件の設定
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_0_48, DBG_NEW DistanceDecision(0.0, 48.0));
	pController->GetDecisionMap().emplace(DecisionId::PLAYER_IN_AREA_DECISION, DBG_NEW PlayerInAreaDecision({ 143,161.6 }, { 401,382.4 }));
	pController->GetDecisionMap().emplace(DecisionId::PLAYER_OUT_AREA_DECISION, DBG_NEW PlayerInAreaDecision({ 143,161.6 }, { 401,382.4 }, true));

	FSM::Transition transitionPtoC(pController->GetDecisionMap()[PLAYER_IN_AREA_DECISION].get(), pController->GetStateMap()[CHASE_STATE].get(), pController->GetStateMap()[PATROL_STATE].get());
	FSM::Transition transitionCtoP(pController->GetDecisionMap()[PLAYER_OUT_AREA_DECISION].get(), pController->GetStateMap()[PATROL_STATE].get(), pController->GetStateMap()[CHASE_STATE].get());
	FSM::Transition transitionCtoA(pController->GetDecisionMap()[DISTANCE_DECISION_0_48].get(), pController->GetStateMap()[ATTACK_STATE].get(), pController->GetStateMap()[CHASE_STATE].get());
	FSM::Transition transitionAtoC(pController->GetDecisionMap()[PLAYER_IN_AREA_DECISION].get(), pController->GetStateMap()[CHASE_STATE].get(), pController->GetStateMap()[ATTACK_STATE].get());
	FSM::Transition transitionAtoP(pController->GetDecisionMap()[PLAYER_OUT_AREA_DECISION].get(), pController->GetStateMap()[PATROL_STATE].get(), pController->GetStateMap()[ATTACK_STATE].get());

	//パトロールステート
	pController->GetStateMap()[PATROL_STATE].get()->GetEnterActions().push_back(pController->GetActionMap()[PATROL_IN_RECT_AREA_ONCE_ACTION].get());
	pController->GetStateMap()[PATROL_STATE].get()->GetActions().push_back(pController->GetActionMap()[PATROL_IN_RECT_AREA_ACTION].get());
	pController->GetStateMap()[PATROL_STATE].get()->GetTransitions().push_back(transitionPtoC);
	//追跡ステート
	pController->GetStateMap()[CHASE_STATE].get()->GetEnterActions().push_back(pController->GetActionMap()[SET_AROUND_INDEX_ACTION].get());
	pController->GetStateMap()[CHASE_STATE].get()->GetActions().push_back(pController->GetActionMap()[CHASE_AROUND_ACTION].get());
	pController->GetStateMap()[CHASE_STATE].get()->GetTransitions().push_back(transitionCtoP);
	pController->GetStateMap()[CHASE_STATE].get()->GetTransitions().push_back(transitionCtoA);
	//攻撃ステート
	pController->GetStateMap()[ATTACK_STATE].get()->GetActions().push_back(pController->GetActionMap()[ATTACK_ACTION].get());
	pController->GetStateMap()[ATTACK_STATE].get()->GetTransitions().push_back(transitionAtoP);
	pController->GetStateMap()[ATTACK_STATE].get()->GetTransitions().push_back(transitionAtoC);

	pController->SetCurrentState(pController->GetStateMap()[PATROL_STATE].get());
	return pController;
}

StateController* AIFactory::SetupStateMachineSkeletonKnightWhite()
{
	StateController* pController = DBG_NEW StateController();
	//ステートの設定
	pController->GetStateMap().emplace(StateId::PATROL_STATE, DBG_NEW State);
	pController->GetStateMap().emplace(StateId::CHASE_STATE, DBG_NEW State);
	pController->GetStateMap().emplace(StateId::ATTACK_STATE, DBG_NEW State);
	//アクションの設定
	pController->GetActionMap().emplace(ActionId::PATROL_ACTION, DBG_NEW PatrolAction);
	pController->GetActionMap().emplace(ActionId::CHASE_AROUND_ACTION, DBG_NEW ChaseAroundAction);
	pController->GetActionMap().emplace(ActionId::SET_AROUND_INDEX_ACTION, DBG_NEW SetAroundIndexAction);
	pController->GetActionMap().emplace(ActionId::ATTACK_ACTION, DBG_NEW AttackAction);
	//遷移と遷移条件の設定
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_160_INF, DBG_NEW DistanceDecision(160.0, INFINITY));
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_80_160, DBG_NEW DistanceDecision(50.0, 160.0));
	pController->GetDecisionMap().emplace(DecisionId::DISTANCE_DECISION_0_80, DBG_NEW DistanceAndObstacleDecision(0.0, 50));

	FSM::Transition transitionPtoC(pController->GetDecisionMap()[DISTANCE_DECISION_80_160].get(), pController->GetStateMap()[CHASE_STATE].get(), pController->GetStateMap()[PATROL_STATE].get());
	FSM::Transition transitionCtoP(pController->GetDecisionMap()[DISTANCE_DECISION_160_INF].get(), pController->GetStateMap()[PATROL_STATE].get(), pController->GetStateMap()[CHASE_STATE].get());
	FSM::Transition transitionCtoA(pController->GetDecisionMap()[DISTANCE_DECISION_0_80].get(), pController->GetStateMap()[ATTACK_STATE].get(), pController->GetStateMap()[CHASE_STATE].get());
	FSM::Transition transitionAtoC(pController->GetDecisionMap()[DISTANCE_DECISION_80_160].get(), pController->GetStateMap()[CHASE_STATE].get(), pController->GetStateMap()[ATTACK_STATE].get());

	//パトロールステート
	pController->GetStateMap()[PATROL_STATE].get()->GetActions().push_back(pController->GetActionMap()[PATROL_ACTION].get());
	pController->GetStateMap()[PATROL_STATE].get()->GetTransitions().push_back(transitionPtoC);
	//追跡ステート
	pController->GetStateMap()[CHASE_STATE].get()->GetEnterActions().push_back(pController->GetActionMap()[SET_AROUND_INDEX_ACTION].get());
	pController->GetStateMap()[CHASE_STATE].get()->GetActions().push_back(pController->GetActionMap()[CHASE_AROUND_ACTION].get());
	pController->GetStateMap()[CHASE_STATE].get()->GetTransitions().push_back(transitionCtoP);
	pController->GetStateMap()[CHASE_STATE].get()->GetTransitions().push_back(transitionCtoA);
	//攻撃ステート
	pController->GetStateMap()[ATTACK_STATE].get()->GetActions().push_back(pController->GetActionMap()[ATTACK_ACTION].get());
	pController->GetStateMap()[ATTACK_STATE].get()->GetTransitions().push_back(transitionAtoC);


	pController->SetCurrentState(pController->GetStateMap()[PATROL_STATE].get());
	return pController;
}

StateController* AIFactory::SetupStateMachineFromFile(const String& i_path)
{
	if (pFsmGraphForLoad_ == nullptr)
	{
		pFsmGraphForLoad_ = DBG_NEW FSMGraph();
	}
	FSMGraphSaveLoad* pSaveLoad = FSMGraphSaveLoad::Get();
	pSaveLoad->LoadGraph( pFsmGraphForLoad_, i_path.narrow());

	StateController* pController =  FSMCompiler::Get()->CompileFromGraph(pFsmGraphForLoad_);
	return pController;
}


