#pragma once
//ヒットエフェクト構造体
struct HitEffectSword : IEffect
{
	//エフェクトの生成位置
	Vec2 pos;
	int effectIndex = 0;
	//エフェクトテクスチャ
	const Texture hitEffectSwordTexture{ U"Assets/pipo-btleffect0872.png" };
	//コンストラクタ
	HitEffectSword(const Vec2& i_pos) : pos(i_pos) {}
	bool update(double t) override
	{		
		hitEffectSwordTexture((128 * effectIndex), 0, 128, 64).drawAt(pos);
		effectIndex++;

		return effectIndex < 9;
	}
};

struct HealEffect : IEffect
{
	//エフェクトの生成位置
	Vec2 pos;
	Actor* owner;
	int effectIndex = 0;
	//エフェクトテクスチャ
	const Texture healEffectTexture{ U"Assets/pipo-btleffect107d.png" };
	//コンストラクタ
	HealEffect(const Vec2& i_pos, Actor* i_owner) : pos(i_pos), owner(i_owner) {}
	bool update(double t) override
	{
		Vec2 pos = owner->GetPosition();
		int index = effectIndex / 2;
		healEffectTexture((120 * index), 0, 120, 120).resized(80).drawAt(pos);
		effectIndex++;

		return index < 10;
	}
};

struct DeathEffect : IEffect
{
	//エフェクトの生成位置
	Actor* owner;
	int effectIndex = 0;
	//エフェクトテクスチャ
	const Texture healEffectTexture{ U"Assets/pipo-btleffect008.png" };
	//コンストラクタ
	DeathEffect( Actor* i_owner) :  owner(i_owner) {}
	bool update(double t) override
	{
		Vec2 pos = owner->GetPosition();
		int index = effectIndex * 0.16;
		healEffectTexture((120 * index), 0, 120, 120).resized(80).drawAt(pos);
		effectIndex++;

		return index < 7;
	}
};

