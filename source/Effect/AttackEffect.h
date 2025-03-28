#pragma once
#include "Actor/Attack.h"
//攻撃エフェクト構造体
struct AttackEffect : IEffect
{
	//所有者Attackクラス
	Attack* pOwner;
	//エフェクトの生成位置
	Vec2 pos;
	//エフェクトの方向(0,1,2,3)
	int32 direction;
	//エフェクトテクスチャ
	const Texture attackEffectTexture{ U"Assets/SlashSpecial1-64.png" };	
	//コンストラクタ
	AttackEffect( Attack* i_pOwner, int32 i_dir) : pOwner(i_pOwner), direction(i_dir) {}
	bool update(double t) override
	{
		//エフェクトを順番に再生
		int32 effectIndex = 0;
		if (t < 0.1) {
		}
		else if (t < 0.2) {
			effectIndex = 1;
		}
		else {
			effectIndex = 2;
		}
		attackEffectTexture((64 * effectIndex), 0, 64, 64).rotated((90_deg * direction)).drawAt(pOwner->GetPosition());
		//エフェクトの再生時間
		return t < 0.3;
	}
};

//スケルトン攻撃エフェクト構造体
struct AttackEffectSkeleton : IEffect
{
	//所有者Attackクラス
	Attack* pOwner;
	//エフェクトの生成位置
	Vec2 pos;
	//エフェクトの方向(0,1,2,3)
	int32 direction;
	//エフェクトテクスチャ
	const Texture attackEffectTexture{ U"Assets/SlashSpecial1-64-skeleton-knight.png" };
	//コンストラクタ
	AttackEffectSkeleton(Attack* i_pOwner, int32 i_dir) : pOwner(i_pOwner), direction(i_dir) {}
	bool update(double t) override
	{
		//エフェクトを順番に再生
		int32 effectIndex = 0;
		if (t < 0.1) {
		}
		else if (t < 0.2) {
			effectIndex = 1;
		}
		else {
			effectIndex = 2;
		}
		attackEffectTexture((64 * effectIndex), 0, 64, 64).rotated((90_deg * direction)).drawAt(pOwner->GetDesignatedPos());
		//エフェクトの再生時間
		return t < 0.3;
	}
};

struct	RangeAttackEffectSkeleton : IEffect
{
	//エフェクトの生成位置
	Vec2 pos;
	int effectIndex = 0;
	//エフェクトテクスチャ
	const Texture hitEffectSwordTexture{ U"Assets/pipo-btleffect018.png" };
	//コンストラクタ
	RangeAttackEffectSkeleton(const Vec2& i_pos) : pos(i_pos) {}
	bool update(double t) override
	{
		int index = effectIndex / 2;
		hitEffectSwordTexture((240 * index), 0, 240, 240).drawAt(pos);
		effectIndex++;

		return index < 8;
	}
};
