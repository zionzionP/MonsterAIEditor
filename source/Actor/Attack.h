#pragma once
#include "Actor/Actor.h"

//Knight,SkeletonKnightに所有、攻撃判定が付けるAttackActor
class Attack :public Actor
{
public:
	Attack(){}
	~Attack() = default;
	Actor* GetOwner() { return pOwner; }
	void SetOwner(Actor* i_pActor) { pOwner = i_pActor; }
private:
	Actor* pOwner = nullptr;
};
