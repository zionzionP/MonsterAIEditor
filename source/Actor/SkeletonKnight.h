#pragma once
#include "Actor/Enemy.h"
#include "Actor/Attack.h"
#include "Effect/AttackEffect.h"
class SkeletonKnight : public Enemy
{
public:
	SkeletonKnight()
	{
		enemyTexture = Texture{ U"Assets/skeleton-knight.png" };
		CollisionComponent* pComponent = pAttackActor_->AddComponent<CollisionComponent, COLLISION>();
		pComponent->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
		pComponent->Edit().collisionTo = Common::CollisionTag::NOTHING;
		pComponent->Edit().hitbox = { Arg::center(0,0), 50, 36 };
		pComponent->Edit().isActivate = false;
		pComponent->Edit().hpEffect = Common::Skeleton_Attack;
	}
	~SkeletonKnight()
	{
		pAttackActor_->SetDead(true);
	}
	const bool GetIsPhaseTransformed() const { return  isPhaseTransformed; }
	void Update(double i_deltaTime) override;
	void UpdateAttack(double i_deltaTime) override;
	void UpdateRender(uint64 i_millisecTime) override;
	void UpdateHPBar()override;
	//第二形態に移行
	void PhaseTransform();
	void SetAttackInActive() override;
	void UpdateEffect() override { effect_.update(); }
	

protected:
	virtual void addAttackEffect() { effect_.add<AttackEffectSkeleton>(pAttackActor_, movement_.effectDirection); }
	//攻撃用Actor
	Attack* pAttackActor_ = DBG_NEW Attack;
	//攻撃前の待ち時間
	double preAttackTime_ = 0.3;
	double preAttackTimer_ = 0.0;
	double ATKTimer = 0.0;
	Texture auraTexture{ U"Assets/pipo-btleffect014.png" };
	//形態変換フラグ
	bool isPhaseTransformed = false;
};
