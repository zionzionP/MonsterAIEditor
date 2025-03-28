#pragma once
#include "AIGraph/StateMachine/Transition.h"
#include "AIGraph/StateMachine/Action/Action.h"
#include "AIGraph/StateMachine/Decision/Decision.h"
#include "AIGraph/StateMachine/StateController.h"
class Action;
struct FSM::Transition;
class Decision;
class StateController;
class State
{
public:
	State() {}
	~State() = default;
	//アクションリストを取得
	const std::vector<Action*>& GetActions() const { return actions_; }
	//アクションリストを取得(非const)
	std::vector<Action*>& GetActions() { return actions_; }
	//進入時アクションリストを取得
	const std::vector<Action*>& GetEnterActions() const { return enterActions_; }
	//進入時アクションリストを取得(非const)
	std::vector<Action*>& GetEnterActions() { return enterActions_; }
	//退出時アクションリストを取得
	const std::vector<Action*>& GetExitActions() const { return exitActions_; }
	//退出時アクションリストを取得(非const)
	std::vector<Action*>& GetExitActions() { return exitActions_; }
	//遷移リストを取得
	const std::vector<FSM::Transition>& GetTransitions() const { return transitions_; }
	//遷移リストを取得(非const)
	std::vector<FSM::Transition>& GetTransitions() { return transitions_; }

	void UpdateState(StateController* io_pController);
	void DoEnter(StateController* io_pController);
	void DoExit(StateController* io_pController);
	void DoAction(StateController* io_pController);
	void CheckTransition(StateController* io_pController);


private:
	std::vector<Action*> actions_;
	std::vector<Action*> enterActions_;
	std::vector<Action*> exitActions_;
	std::vector<FSM::Transition> transitions_;


};
