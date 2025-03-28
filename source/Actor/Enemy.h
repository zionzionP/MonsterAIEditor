#pragma once
#include <Siv3D.hpp>
#include "Actor/Actor.h"
#include "Core/Common.h"
#include "Component/CollisionComponent.h"
#include "Navigation/NavAgent.h"
#include "AIGraph/StateMachine/StateController.h"
#include "Effect/HitEffect.h"

class StateController;
class NavAgent;

//移動用パラメータ
struct EnemyMovement
{
	EnemyMovement() :
		maxSpeed(200.0),
		currentSpeed(100.0),
		velocity{ 0, 0 },
		direction(0),
		effectDirection(0)
	{}
	//最大速度
	double maxSpeed;
	//現在の速度
	double currentSpeed;
	//速度ベクトル
	Vec2 velocity;
	//キャラクターの向き(0:下　1:左　2:右　3:上)
	int32 direction;
	//エフェクトの向き(0:下　1:左　2:上　3:右)
	int32 effectDirection;
};

//敵キャラクタークラス
class Enemy : public Actor
{
public:
	Enemy()
	{
		ActorManager::Get()->GetEnemies().push_back(this);
	}
	virtual ~Enemy()
	{
		if (pController_)
		{
			delete pController_;
		}
		auto& enemies = ActorManager::Get()->GetEnemies();
		enemies.erase(std::find(enemies.begin(), enemies.end(), this));
	}
	//ターゲットを取得
	const Actor* GetTarget() const { return pTarget_; }
	//ターゲットをセット
	void SetTarget(Actor* i_pTarget) { pTarget_ = i_pTarget; }
	//移動の目標位置を取得
	const Vec2& GetTargetPos() const { return wayPos_; }
	//移動の目標位置をセット
	void SetTargetPos(const Vec2& i_pos) { wayPos_ = i_pos; }
	//移動の目標位置を取得
	const Vec2& GetGoalPos() const { return goalPos_; }
	//移動の目標位置をセット
	void SetGoalPos(const Vec2& i_goalPos) { goalPos_ = i_goalPos; }
	//突進攻撃の方向をセット
	void SetAttackVec2(const Vec2& i_vec2) { attackVec2_ = i_vec2; }
	//攻撃中フラグをゲット
	bool GetIsAttacking() { return isAttacking_; }
	//攻撃中フラグをセット
	void SetIsAttacking(bool in) { isAttacking_ = in; }
	//移動終点にしているプレイヤーの周りにある位置の番号をゲット
	int GetAroundPosIndex() { return aroundPosIndex_; }
	//移動終点にしているプレイヤーの周りにある位置の番号をセット
	void SetAroundPosIndex(int in) { aroundPosIndex_ = in; }
	//移動パラメータを取得
	const EnemyMovement& GetMovement() const { return movement_; }
	//移動パラメータを取得(非const)
	EnemyMovement& GetMovement() { return movement_; }
	//ナビゲーションエージェントを取得(非const)
	NavAgent& GetAgent() { return agent_; }
	//Setステートマシン
	void SetStateController(StateController* i_pController) { pController_ = i_pController; }
	StateController* GetStateController()const { return pController_; }
	//ノックバック処理
	bool KnockBackDone(double i_deltaTime);
	virtual void Update(double i_deltaTime) override;
	//攻撃される時の処理
	void OnHitted(int i_collisionTag,const Actor* i_pOther, int i_hpEffect, int i_attackId) override;
	//攻撃される後の処理
	void UpdateHitting(double i_deltaTime);
	//HPバーを更新
	virtual void UpdateHPBar();	
	//死亡時処理
	bool UpdateDeath();
	//移動処理
	void UpdateMove(double i_deltaTime);
	//経路上に次の位置を設定
	void UpdateWayPos();
	//ナビゲーションエージェントを利用して開始位置と終点の経路を算出
	void UpdatePath();
	//突進攻撃処理
	virtual void UpdateAttack(double i_deltaTime);
	bool AttackDone(double i_deltaTime);
	//描画
	virtual void UpdateRender(uint64 i_millisecTime) override;

	void UpdateEffect() override { effect_.update(); }

	virtual void SetAttackInActive(){}

protected:
	//移動パラメータ
	EnemyMovement movement_;
	//ターゲットに向かって移動する
	Actor* pTarget_ = nullptr;
	//移動中向かう中間Position
	Vec2 wayPos_ = position_;
	//移動に向かう最終Position
	Vec2 goalPos_ = position_;
	RectF HPBar_{ {position_.x - 14,position_.y - 21 }, 28,3 };
	NavAgent agent_;
	//パスリストの何番目まで進んでいる
	int pathIndex_ = 0;
	//経路探索のタイマー
	double findPathTimer_ = 1.0;
	//ステートマシン
	StateController* pController_ = nullptr;
	//攻撃中フラグ
	bool isAttacking_ = false;
	//HPが0の状態でtrueになる、死亡動画を再生
	bool isDying_ = false;
	double atkTimer_ = 0.0;
	Vec2 attackVec2_{ 0,0 };
	int aroundPosIndex_ = 0;
	//キャラクターのテキスチャー
	Texture enemyTexture{ U"Assets/pipo-charachip019.png" };
	Effect effect_;
};
