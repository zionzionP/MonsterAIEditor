#include "Level/GameLevel.h"
void GameLevel::Start()
{
	isLevelCleared = false;
	pCManager->AddFactory(COLLISION, DBG_NEW CollisionComponentFactory);
	setup.SetupGameLevel();
}

void GameLevel::Update()
{
	//デルタ時間
	deltaTime = Scene::DeltaTime();
	//アプリ起動後の時間(ミリ秒単位)
	millisecTime = Time::GetMillisec();
	//背景を描画
	renderTexture.clear(Palette::Darkgray).draw();
	//壁コリジョンの描画
	pManager->DrawWallCollision();

	if (isUpdate)
	{
		//更新処理
		pManager->Update(deltaTime);
		//Component更新処理
		pCManager->Update();
		//designatedPosをpositionに適用する
		pManager->UpdatePosition();
	}		

	//描画処理		
	pManager->UpdateRender(millisecTime);

	
	
	if (setup.GetPlayer().GetHP() <= 0)
	{
		isUpdate = false;
		renderTexture.clear(ColorF{ 0.0, 0.0, 0.0, 0.9 }).draw();
		font(U"YOU DIED").drawAt(50, Vec2{ 320, 150 }, Palette::Red);
		font(U"Press R to Retry").drawAt(20, Vec2{ 320, 380 }, Palette::White);

		if (KeyR.down())
		{
			LevelManager::Get()->ChangeLevel(GAME_LEVEL_STAGE1);
		}
	}

	if (static_cast<int>(pManager->GetActors().size()) <= setup.GetActorsNum())
	{
		isLevelCleared = true;
	}

	UpdatePause();
	
}

void GameLevel::UpdatePause()
{
	//一時停止
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
	if (isPaused)
	{
		renderTexture.clear(ColorF{ 0.0, 0.0, 0.0, 0.9 }).draw();
		font(U"PAUSE").drawAt(50, Vec2{ 320, 150 }, Palette::White);
		font(U"Press T to Title").drawAt(20, Vec2{ 320, 380 }, Palette::White);		
		if (KeyT.down())
		{
			isPaused = false;
			isUpdate = true;
			LevelManager::Get()->ChangeLevel(TITLE_LEVEL);
		}
	}
}

void GameLevel::UpdateRenderWarpGate(const Vec2& i_pos)
{
	int index = effectIndex * 0.2;
	warpGateEffectTexture(96 * (index % 5), (index / 5) * 96, 96, 96).drawAt(i_pos);	
	if (index >= 9)
	{
		effectIndex = 0;
	}else
	{
		effectIndex++;
	}
}

void GameLevel::UpdateRenderArrowToGate(const Vec2& i_pos)
{
	Vec2 playerToClearPoint = i_pos - setup.GetPlayer().GetPosition();
	double x = playerToClearPoint.x;
	double y = playerToClearPoint.y;
	Vec2 vec2ArrowDirection {0,0};
	int direction = 0;
	if(std::abs(x) > std::abs(y))
	{
		if(x >= 0)
		{
			direction = 3;
			vec2ArrowDirection = { 1,0 };
		}
		else
		{
			direction = 1;
			vec2ArrowDirection = { -1,0 };
		}
	}else
	{
		if(y >= 0)
		{
			direction = 0;
			vec2ArrowDirection = { 0,1 };
		}
		else
		{
			direction = 2;
			vec2ArrowDirection = { 0,-1 };
		}
	}
	const int distanceFromPlayer = 40;
	Vec2 renderPos = setup.GetPlayer().GetPosition() + distanceFromPlayer * vec2ArrowDirection;
	int index = arrowEffectIndex * 0.2;
	arrowEffectTexture(0, (index % 6) * 16, 30, 16).rotated((90_deg * direction)).drawAt(renderPos);
	if (index >= 5)
	{
		arrowEffectIndex = 0;
	}else
	{
		arrowEffectIndex++;
	}
}

void GameLevel::CheckLevelChange()
{
	
}
void GameLevel::Exit()
{	
	pManager->ClearActor();
}
