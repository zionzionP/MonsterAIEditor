#pragma once
#include "Level/GameLevel.h"
class GameLevelStage2 : public GameLevel
{
public:
	GameLevelStage2() {}
	~GameLevelStage2() = default;

	void Start() override
	{
		isLevelCleared = false;
		isUpdate = true;
		pCManager->AddFactory(COLLISION, DBG_NEW CollisionComponentFactory);
		setup.SetupGameLevelStage2(startPoint);
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
			setup.GetMap().UpdateMapRender(LOOK_STAGE2_LAYER1);
			setup.GetMap().UpdateMapRender(LOOK_STAGE2_LAYER2);
			if (isLevelCleared)
			{
				UpdateRenderWarpGate(clearPoint);
			}
			//描画処理		
			pManager->UpdateRender(millisecTime);
			setup.GetMap().UpdateMapRender(LOOK_STAGE2_OVER);
			pManager->UpdateEffect();
			if (isLevelCleared)
			{
				UpdateRenderArrowToGate(clearPoint);
			}
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
				LevelManager::Get()->ChangeLevel(GAME_LEVEL_STAGE2);
			}
		}

		if (pManager->GetEnemies().size() <= 0)
		{
			isLevelCleared = true;
		}

		UpdatePause();

	}
	void CheckLevelChange() override
	{
		if (isLevelCleared)
		{
			const int distanceSq = 32*32;
			if (clearPoint.distanceFromSq(setup.GetPlayer().GetPosition()) < distanceSq)
			{
				LevelManager::Get()->SetPlayerHP(setup.GetPlayer().GetHP());
				LevelManager::Get()->ChangeLevel(GAME_LEVEL_STAGE3);
			}
		}
	}

private:
	Vec2 startPoint{ 16,224 };
	Vec2 clearPoint{ 608,224 };
};
