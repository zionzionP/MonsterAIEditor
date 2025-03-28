#pragma once
#include "Level/GameLevel.h"
class GameLevelStage3 : public GameLevel
{
public:
	GameLevelStage3() {}
	~GameLevelStage3() = default;

	void Start() override
	{
		isLevelCleared = false;
		isUpdate = true;
		pCManager->AddFactory(COLLISION, DBG_NEW CollisionComponentFactory);
		setup.SetupGameLevelStage3(startPoint);
		setup.GetPlayer().SetHP(LevelManager::Get()->GetPlayerHP());
	}
	void Update() override
	{
		//デルタ時間
		deltaTime = Scene::DeltaTime();
		//アプリ起動後の時間(ミリ秒単位)
		millisecTime = Time::GetMillisec();		
		
		if (isUpdate)
		{
			//更新処理
			pManager->Update(deltaTime);
			//Component更新処理
			pCManager->Update();
			//designatedPosをpositionに適用する
			pManager->UpdatePosition();
		}

		//カメラに乗る描画処理
		{
			if (&setup.GetPlayer() != nullptr)
			{
				camera.jumpTo(setup.GetPlayer().GetPosition(), 2);
			}
			auto t = camera.createTransformer();
			//背景を描画
			renderTexture.clear(Palette::Darkgray).draw();
			//マップを描画
			setup.GetMap().UpdateMapRender(LOOK_STAGE3_LAYER1);
			setup.GetMap().UpdateMapRender(LOOK_STAGE3_LAYER2);

			//描画処理		
			pManager->UpdateRender(millisecTime);
			setup.GetMap().UpdateMapRender(LOOK_STAGE3_OVER);
			pManager->UpdateEffect();
		}
		//カメラに乗らない描画処理(UI)
		if (&setup.GetPlayer() != nullptr)
		{
			setup.GetPlayer().RenderHPBar();
		}

		if (setup.GetPlayer().GetHP() <= 0)
		{
			isUpdate = false;
			renderTexture.clear(ColorF{ 0.0, 0.0, 0.0, 0.9 }).draw();
			font(U"YOU DIED").drawAt(50, Vec2{ 320, 150 }, Palette::Red);
			font(U"Press R to Retry").drawAt(20, Vec2{ 320, 380 }, Palette::White);

			if (KeyR.down())
			{
				LevelManager::Get()->SetPlayerHP(Common::PlayerMaxHP);
				LevelManager::Get()->ChangeLevel(GAME_LEVEL_STAGE3);
			}
		}

		int enemiesNum = static_cast<int>(pManager->GetEnemies().size());
				
		if (enemiesNum == 1)
		{
			SkeletonKnight* lastOneSkeleton = static_cast<SkeletonKnight*>(pManager->GetEnemies()[0]);
			if (!lastOneSkeleton->GetIsPhaseTransformed())
			{
				lastOneSkeleton->PhaseTransform();
			}
		}
		else if(enemiesNum <= 0)
		{
			isLevelCleared = true;
		}

		UpdatePause();

	}
	void CheckLevelChange() override
	{		
		if (isLevelCleared)
		{
			isUpdate = false;
			renderTexture.clear(ColorF{ 0.0, 0.0, 0.0, 0.9 }).draw();
			font(U"STAGE CLEAR").drawAt(50, Vec2{ 320, 150 }, Palette::White);
			font(U"Press R to Retry").drawAt(20, Vec2{ 320, 380 }, Palette::White);
			font(U"Press T to Title").drawAt(20, Vec2{ 316.5, 400 }, Palette::White);
			if (KeyR.down())
			{
				LevelManager::Get()->ChangeLevel(GAME_LEVEL_STAGE1);
			}
			if (KeyT.down())
			{
				LevelManager::Get()->ChangeLevel(TITLE_LEVEL);
			}
		}
	}
private:
	Vec2 startPoint{ 16,48 };
};
