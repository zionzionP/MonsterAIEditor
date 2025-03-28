#pragma once
#include <Siv3D.hpp>
#include <string>
#include "Core/Common.h"
#include "Effect/AttackEffect.h"
#include "Actor/Actor.h"
#include "Actor/Attack.h"
#include "Component/CollisionComponent.h"
#include "Component/ComponentFactoryManager.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif
//キャラクターの向き、順番はアセットに従うために下、左、右、上
enum ECharacterDirection {
	CDOWN = 0,
	CLEFT,
	CRIGHT,
	CUP
};
//エフェクトの向き、順番は下から時計回り下、左、上、右
enum EEffectDirection {
	EDOWN = 0,
	ELEFT,
	EUP,
	ERIGHT
};
//移動用パラメータ
struct MovementParameters
{
	MovementParameters():
		maxSpeed(200.0),
		currentSpeed(180),
		velocity{ 0, 0 },
		direction(ECharacterDirection::CDOWN),
		effectDirection(EEffectDirection::EDOWN){}

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

//ナイトキャラクタークラス
class Knight : public Actor
{
public:
	Knight()
	{
		pAttackActor_->SetOwner(this);
		CollisionComponent* pComponent = pAttackActor_->AddComponent<CollisionComponent, COLLISION>();
		pComponent->Edit().collisionFlags = Common::CollisionTag::COLLISION_ATTACKEFFECT;
		pComponent->Edit().collisionTo = Common::CollisionTag::NOTHING;
		pComponent->Edit().hitbox = { Arg::center(0,0), 50, 36 };
		pComponent->Edit().isActivate = false;
		pComponent->Edit().hpEffect = Common::Knight_Attack;

	}
	~Knight() = default;
	//キャラクターの移動パラメータを取得
	const MovementParameters& GetMovement() const { return movement_ ; }
	//AttackActorを取得
	Attack& EditAttack() { return *pAttackActor_; }
	//移動を更新
	void UpdateMove(double i_deltaTime);
	//攻撃を更新
	void UpdateAttack(double i_deltaTime);
	//ノックバック処理
	bool KnockBackDone(double i_deltaTime);
	//コリジョン判定される時の処理
	void OnHitted(int i_collisionTag, const Actor* i_pOther, int i_hpEffect, int i_attackId) override;
	//攻撃される後の処理
	void UpdateHitting(double i_deltaTime);
	//無敵時間を更新
	void UpdateDamageable(double i_deltaTime);
	void UpdateHPBar();
	void RenderHPBar();
	//描画以外の更新処理
	void Update(double i_deltaTime) override;
	//描画
	void UpdateRender(uint64 i_millisecTime) override;

	void UpdateEffect() override{effect_.update();}
private:
	//移動用パラメータ
	MovementParameters movement_;
	//キャラクターのテキスチャー
	const Texture kinghtTexture{ U"Assets/pipo-charachip021.png" };	
	//エフェクト発生装置
	Effect effect_;
	//攻撃用Actor
	Attack* pAttackActor_ = DBG_NEW Attack;
	//HPバー
	RectF HPBar_{ {50,20 }, 200,10 };
	bool isAttacking = false;
	double ATKTimer = 0.0;
	Font font{ FontMethod::MSDF, 50 };
};
