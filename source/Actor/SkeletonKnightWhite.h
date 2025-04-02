#pragma once
#include "Actor/SkeletonKnight.h"
class SkeletonKnightWhite : public SkeletonKnight
{
public:
	SkeletonKnightWhite()
	{
		enemyTexture = Texture{ U"Assets/skeleton-knight-white.png" };
	};
	~SkeletonKnightWhite() = default;

	void addAttackEffect()override { effect_.add<AttackEffect>(pAttackActor_, movement_.effectDirection); }
};
