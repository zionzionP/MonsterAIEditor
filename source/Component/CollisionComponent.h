#pragma once
#include <Siv3D.hpp>
#include "Core/Common.h"
#include "Component/Component.h"
#include "Component/CollisionComponentFactory.h"

//長方形コリジョン判定：壁
//円形コリジョン判定：キャラクター
enum HitboxType
{
	RECT = 0,
	CIRCLE = 1,
};
class CollisionComponentFactory;
//パラメータ
struct CollisionParameters {
	CollisionParameters(int i_type = HitboxType::RECT, RectF i_rect = { Arg::center(0,0), 0, 0 }, Circle i_circle = { Vec2{0,0},0 }, int i_collisionFlags = -1, int i_collisionto = -1, int i_hpEffect = 0, bool i_isActivate = false) :
		hitboxType(i_type),
		hitbox(i_rect),
		hitboxCircle(),
		collisionFlags(i_collisionFlags),
		collisionTo(i_collisionto),
		hpEffect(i_hpEffect),
		isActivate(i_isActivate) {}
	int hitboxType;
	RectF hitbox;
	Circle hitboxCircle;
	int collisionFlags;//自身のコリジョンフラグ
	int collisionTo;//他のどのフラグを持つものに反応する
	int hpEffect;//コリジョン発生時相手のHP変化値
	bool isActivate;
};

class CollisionComponent :public Component
{
public:
	CollisionComponent(Actor* io_actor, CollisionComponentFactory* io_pFactory) :
		Component(io_actor),
		pFactory_(io_pFactory),
		collisionParameters_() {}

	CollisionParameters& Edit() { return collisionParameters_; }
	const CollisionParameters& Get() const { return collisionParameters_; }

private:
	CollisionParameters collisionParameters_;
	CollisionComponentFactory* pFactory_;
};
