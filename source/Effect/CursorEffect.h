#pragma once
struct CursorEffect : IEffect
{
	//エフェクトの生成位置
	Vec2 pos;
	//最大半径
	double radius = 32;
	int index = 0;
	//消えるまでのフレーム数
	int maxIndex = 20;
	//コンストラクタ
	CursorEffect(const Vec2& i_pos) : pos(i_pos) {}
	bool update(double t) override
	{
		Circle{ pos, radius/maxIndex*index }.drawFrame(4,Palette::Black);
		index++;
		return index < maxIndex;
	}
};
