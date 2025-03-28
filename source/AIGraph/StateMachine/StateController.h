#pragma once
#include "Actor/Actor.h"
#include "Actor/Enemy.h"
#include "AIGraph/StateMachine/Action/Action.h"
#include "AIGraph/StateMachine/Decision/Decision.h"
#include "AIGraph/StateMachine/Transition.h"
#include <memory>
#include "AIGraph/StateMachine/State.h"

class State;
class StateController
{
public:
	typedef std::unordered_map<int, std::shared_ptr<State>> StateMap;
	typedef std::unordered_map<int, std::shared_ptr<Decision>> DecisionMap;
	typedef std::unordered_map<int, std::shared_ptr<Action>> ActionMap;

	StateController()
	{
		stateMap_.reserve(20);
		decisionMap_.reserve(20);
		actionMap_.reserve(20);
	}
	~StateController() = default;
	//オーナーアクターを取得
	const Actor& GetOwner() const {	return *pOwnerActor_; }
	//オーナーアクターを取得(非const)
	Actor& GetOwner() { return *pOwnerActor_; }
	void SetOwner(Actor* i_pActor) { pOwnerActor_ = i_pActor; }
	//オーナーエネミーを取得
	const Enemy& GetOwnerEnemy() const { return *pOwnerEnemy_; }
	//オーナーエネミーを取得(非const)
	Enemy& GetOwnerEnemy() { return *pOwnerEnemy_; }
	void SetOwnerEnemy(Enemy* i_pEnemy) { pOwnerEnemy_ = i_pEnemy; }
	//ステートマップを取得
	const auto& GetStateMap() const { return stateMap_; }
	//ステートマップを取得(非const)
	auto& GetStateMap(){ return stateMap_; }
	//Decisionマップを取得
	const auto& GetDecisionMap() const { return decisionMap_; }
	//Decisionマップを取得(非const)
	auto& GetDecisionMap() { return decisionMap_; }
	//Actionマップを取得
	const auto& GetActionMap() const { return actionMap_; }
	//Actionマップを取得(非const)
	auto& GetActionMap() { return actionMap_; }
	void SetCurrentState(State* i_pState) { pCurrentState_ = i_pState; }
	State* GetCurrentState() const { return pCurrentState_; }
	//フレームの経過時間を取得
	const double GetDeltaTime() const { return deltaTime_; }
	//フレームの経過時間を取得(非const)
	double GetDeltaTime() { return deltaTime_; }
	//現在のステートのアクションが終了したかを取得
	bool GetIsCurrentStateActionDone() const { return isCurrentStateActionDone; }
	void SetCurrentStateActionDone(bool in) { isCurrentStateActionDone = in; }
	
	void Update(double i_deltaTime);

	void TransitionToState(State* i_nextState);

private:
	double deltaTime_ = 0.0;
	bool isCurrentStateActionDone = false;
	Actor* pOwnerActor_ = nullptr;
	Enemy* pOwnerEnemy_ = nullptr;
	State* pCurrentState_ = nullptr;
	StateMap stateMap_;
	ActionMap actionMap_;
	DecisionMap decisionMap_;
};
