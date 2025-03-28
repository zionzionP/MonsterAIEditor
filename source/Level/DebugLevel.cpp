#include "Level/DebugLevel.h"

void DebugLevel::Start()
{
	pCManager->AddFactory(COLLISION, DBG_NEW CollisionComponentFactory);
}
void DebugLevel::Update()
{	
	//デルタ時間
	deltaTime = Scene::DeltaTime();
	//アプリ起動後の時間(ミリ秒単位)
	millisecTime = Time::GetMillisec();	
	{
		//背景を描画
		renderTexture.clear(Palette::Darkgray).draw();
		//セットアップでデバッグ機能が使える
		setup.Update();	
		if(isUpdate)
		{
			//更新処理
			pManager->Update(deltaTime);

			//Component更新処理
			pCManager->Update();
			//designatedPosをpositionに適用する
			pManager->UpdatePosition();
		}
		if (&setup.GetPlayer() != nullptr)
		{
			//camera.jumpTo(setup.GetPlayer().GetPosition(), 2);
			/*camera.setTargetScale(2.0);
			camera.setTargetCenter(setup.GetPlayer().GetPosition());
			camera.update();*/
		}
		//auto t = camera.createTransformer();
		
		//マップを描画
		setup.GetMap().UpdateMapRender(LOOK_STAGE3_LAYER1);
		setup.GetMap().UpdateMapRender(LOOK_STAGE3_LAYER2);
		//描画処理		
		pManager->UpdateRender(millisecTime);
		setup.GetMap().UpdateMapRender(LOOK_STAGE3_OVER);
		pManager->UpdateEffect();
	}
	if (&setup.GetPlayer() != nullptr)
	{
		setup.GetPlayer().RenderHPBar();
	}
	font(U"0: 全クリア").draw(15, Vec2{ 450, 15 }, Palette::Black);
	font(U"1: 基本ステージ生成").draw(15, Vec2{ 450, 30 }, Palette::Black);
	font(U"2: 壁生成、正方形20個").draw(15, Vec2{ 450, 45 }, Palette::Black);
	font(U"3: 壁生成、長方形10個").draw(15, Vec2{ 450, 60 }, Palette::Black);
	font(U"4: プレイヤー生成").draw(15, Vec2{ 450, 75 }, Palette::Black);
	font(U"5: 敵1体生成").draw(15, Vec2{ 450, 90 }, Palette::Black);
	font(U"6: プレイヤーHP無限").draw(15, Vec2{ 450, 105 }, Palette::Black);
	font(U"7: ナビメッシュ表示On/Off").draw(15, Vec2{ 450, 120 }, Palette::Black);
	font(U"8: 経路表示On/Off").draw(15, Vec2{ 450, 135 }, Palette::Black);
	font(U"9: タイトルに戻る").draw(15, Vec2{ 450, 150 }, Palette::Black);
	font(U"F4: CSVマップロード").draw(15, Vec2{ 450, 165 }, Palette::Black);
	font(U"F5: 壁のコリジョン表示").draw(15, Vec2{ 450, 180 }, Palette::Black);


	if (Key9.down())
	{		
		LevelManager::Get()->ChangeLevel(TITLE_LEVEL);
	}
	if (KeyP.down())
	{
		if (isPaused)
		{
			isPaused = false;
			isUpdate = true;
		}
		else
		{
			isPaused = true;
			isUpdate = false;
		}
	}
}
void DebugLevel::Exit()
{
	pManager->ClearActor();
}
