#pragma once
#include "MapItem/MapItem.h"

class HealItem :public MapItem
{
public:
	HealItem()
	{
		CollisionComponent* pComponent = this->AddComponent<CollisionComponent, COLLISION>();
		pComponent->Edit().collisionFlags = Common::CollisionTag::COLLISION_HEALITEM;
		pComponent->Edit().collisionTo = Common::CollisionTag::COLLISION_KNIGHT;
		pComponent->Edit().hitboxType = CIRCLE;
		pComponent->Edit().hitboxCircle = { Arg::center(0,0), 12 };
		pComponent->Edit().isActivate = true;
		pComponent->Edit().hpEffect = Common::healPoint;
	}
	~HealItem() = default;

	void UpdateRender(uint64 i_millisecTime) override
	{
		//ミリ秒を割り算し余りでアニメーションの順番を指定
		const int32 animationIndex = patterns[(i_millisecTime / 100 % 4)] + 6;
		healItemTexture((24 * animationIndex), 24, 24, 24).drawAt(position_);
	}
	void OnHitted(int i_collisionTag, const Actor* i_pOther, int i_hpEffect, int i_attackId) override
	{
		isDead_ = true;
	}
	
private:
	const Texture healItemTexture{ U"Assets/sticon_set.png" };
	//アニメーション用配列
	static constexpr int32 patterns[4] = { 0, 1, 2, 1 };
};
