#pragma once
#include "Core/Common.h"


class FSMGraphSelectEnemyButton
{
public:
	FSMGraphSelectEnemyButton(const Vec2& i_pos) :
		position_(i_pos)
	{}
	~FSMGraphSelectEnemyButton() = default;


	bool IsClicked() const{ return buttonRect_.leftClicked(); }
	void SetIsSelected(bool in) { isSelected = in; }

	void SetTexture(const String& path) { enemyTexture = Texture{ path }; }

	void Draw(uint64 i_millisecTime) const
	{
		if (isSelected)
		{
			buttonRect_.drawFrame(4, Palette::Yellow);
			//ミリ秒を割り算し余りでアニメーションの順番を指定
			const int32 n = i_millisecTime / 100 % 4;
			int32 animationIndex = Common::patterns[n];
			enemyTexture((32 * animationIndex), 0, 32, 32).drawAt(position_);
		}
		else
		{
			int32 animationIndex = 1;
			enemyTexture((32 * animationIndex), 0, 32, 32).drawAt(position_);
		}
	}

protected:
	Vec2 position_ ;
	RectF buttonRect_ = { Arg::center(position_),96 };
	bool isSelected = false;
	//enemyのテキスチャー
	Texture enemyTexture{ U"Assets/pipo-charachip019.png" };
};
