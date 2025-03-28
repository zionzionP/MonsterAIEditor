#include "Level/TitleLevel.h"

void TitleLevel::Start()
{
	isInstructionOn = false;
}
void TitleLevel::Update()
{
	renderTexture.clear(Palette::Black).draw();
	if (isInstructionOn)
	{
		RoundRect{ Arg::center(160, 165), 30, 30, 5 }.drawFrame(2, Palette::White);
		font(U"W").drawAt(25, Vec2{ 160, 165 }, Palette::White);
		RoundRect{ Arg::center(128, 197), 30, 30, 5 }.drawFrame(2, Palette::White);
		font(U"A").drawAt(25, Vec2{ 128, 197 }, Palette::White);
		RoundRect{ Arg::center(160, 197), 30, 30, 5 }.drawFrame(2, Palette::White);
		font(U"S").drawAt(25, Vec2{ 160, 197 }, Palette::White);
		RoundRect{ Arg::center(192, 197), 30, 30, 5 }.drawFrame(2, Palette::White);
		font(U"D").drawAt(25, Vec2{ 192, 197 }, Palette::White);
		RoundRect{ Arg::center(350, 164), 30, 30, 5 }.drawFrame(2, Palette::White);
		font(U"J").drawAt(25, Vec2{ 350, 164 }, Palette::White);
		RoundRect{ Arg::center(350, 198), 30, 30, 5 }.drawFrame(2, Palette::White);
		font(U"P").drawAt(25, Vec2{ 350, 198 }, Palette::White);
		font(U" : MOVE").drawAt(30, Vec2{ 260, 197 }, Palette::White);
		font(U" : ATTACK").drawAt(30, Vec2{ 435, 164 }, Palette::White);
		font(U" : PAUSE").drawAt(30, Vec2{ 422.5, 198 }, Palette::White);
		
		font(U"PRESS ENTER BUTTON").drawAt(20, Vec2{ 320, 380 }, Palette::White);
		if (KeyEnter.down())
		{			
			LevelManager::Get()->ChangeLevel(LevelName::GAME_LEVEL_STAGE1);
		}
	}
	if (!isInstructionOn)
	{
		font(U"2DAT").drawAt(50, Vec2{ 320, 150 }, Palette::White);
		font(U"PRESS ENTER BUTTON").drawAt(20, Vec2{ 320, 380 }, Palette::White);
		//font(U"Press F2 to enter DEBUG").drawAt(15, Vec2{ 550, 470 }, Palette::White);
		if (KeyEnter.down())
		{
			isInstructionOn = true;
		}
		if (KeyF2.down())
		{
			//Debug機能
			LevelManager::Get()->ChangeLevel(LevelName::DEBUG_LEVEL);
		}

		if (KeyF3.down())
		{
			//editor
			LevelManager::Get()->ChangeLevel(LevelName::EDITOR_LEVEL);
		}
	}	
}
