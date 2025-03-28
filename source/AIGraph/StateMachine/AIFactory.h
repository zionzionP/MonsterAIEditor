#pragma once
#include "AIGraph/StateMachine/StateController.h"
#include "Core/Singleton.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

class StateController;

enum StateId {
	PATROL_STATE = 0,//パトロール
	CHASE_STATE = 1,//追跡
	ATTACK_STATE = 2,//攻撃
};
enum DecisionId {
	DISTANCE_DECISION_80_160 = 0,//プレイヤーとの距離は64～160か
	DISTANCE_DECISION_0_80 = 1,//プレイヤーとの距離は0～64か
	DISTANCE_DECISION_160_INF = 2,//プレイヤーとの距離は160～か
	REACH_AROUNDPOS_DECISION = 3,//プレイヤー周りのマスに到達してるか
	DISTANCE_DECISION_0_48 = 4,//プレイヤーとの距離は0～48か
	DISTANCE_DECISION_48_160 = 5,//プレイヤーとの距離は48～160か
	PLAYER_OUT_PATROL_AREA_DECISION = 6,//プレイヤーは円形のパトロールエリアから離れたか
	PLAYER_IN_PATROL_AREA_DECISION = 7,//プレイヤーは円形のパトロールエリアに侵入したか
	PLAYER_IN_AREA_DECISION = 8,//プレイヤーは長方形のパトロールエリアに侵入したか
	PLAYER_OUT_AREA_DECISION = 9,//プレイヤーは長方形のパトロールエリアから離れたか
};
enum ActionId {
	PATROL_ACTION = 0,//自身周囲一定半径内パトロールする
	CHASE_ACTION = 1,//プレイヤーの位置に向かって追跡する
	ATTACK_ACTION = 2,//攻撃
	CHASE_AROUND_ACTION = 3,//プレイヤーの周りの位置に向かって追跡する
	SET_AROUND_INDEX_ACTION = 4,//プレイヤー周りの位置を1個指定する
	PATROL_IN_AREA_ACTION = 5,//指定された位置の周りに一定半径内パトロールする
	PATROL_IN_AREA_ONCE_ACTION = 6,//指定された位置の周りに一定半径内パトロールする(一回だけ)
	PATROL_IN_RECT_AREA_ACTION = 7,//指定された長方形のエリア内パトロールする
	PATROL_IN_RECT_AREA_ONCE_ACTION = 8,//指定された長方形のエリア内パトロールする(一回だけ)
};
//ステートマシンを設置するクラス
class AIFactory : public Singleton<AIFactory>
{
public :
	AIFactory(){}
	~AIFactory() = default;

	void RegisterState(const int32& i_index, StateController* io_pController);

	template<class T, typename... Args>
	void RegisterAction(const int32& i_index, StateController* io_pController, Args&&... args)
	{
		io_pController->GetActionMap().emplace(i_index, DBG_NEW T(std::forward<Args>(args)...));
	}

	template<class T, typename... Args>
	void RegisterDecision(const int32& i_index, StateController* io_pController, Args&&... args)
	{
		io_pController->GetDecisionMap().emplace(i_index, DBG_NEW T(std::forward<Args>(args)...));
	}


	//ゴーストタイプエネミー　基本ステートマシン
	StateController* SetupStateMachine();
	//ゴーストタイプエネミー　基本ステートマシン上に、プレイヤーの周りの位置に追跡するので、ばらつきができる
	StateController* SetupStateMachineType2();
	//スケルトンタイプエネミー
	StateController* SetupStateMachineSkeletonKnight();
	
};

