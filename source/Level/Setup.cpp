#include "Level/Setup.h"

#include "Level/LevelManager.h"

Actor* Setup::GenerateWall(const Vec2& i_pos, double i_w, double i_h)
{
	Wall* pWall = DBG_NEW Wall();
	pWall->SetPosition(i_pos);
	pWall->SetDesignatedPos(i_pos);
	CollisionComponent* pCollisionW = pWall->AddComponent<CollisionComponent, COLLISION>();
	pCollisionW->Edit().hitbox = { Arg::center(pWall->GetPosition()), i_w, i_h };
	pCollisionW->Edit().collisionFlags = Common::CollisionTag::COLLISION_WALL;
	pCollisionW->Edit().collisionTo = Common::CollisionTag::NOTHING;
	pCollisionW->Edit().isActivate = true;
	return pWall;
}

bool IsObstacle(const RectF& rect)
{
	ActorManager* pManager = ActorManager::Get();
	for (auto& pActor : pManager->GetActors())
	{
		CollisionComponent* pCollision = pActor->GetComponent<CollisionComponent, COLLISION>();
		if (pCollision != nullptr)
		{
			if (pCollision->Get().hitbox.intersects(rect))
			{
				return true;
			}
		}
	}
	return false;
}
void Setup::GeneRandomMap(int num)
{
	Actor* pWallT = GenerateWall({ -16,240 }, 32, 480);
	Actor* pWallB = GenerateWall({ 656,240 }, 32, 480);
	Actor* pWallL = GenerateWall({ 320,-16 }, 640, 32);
	Actor* pWallR = GenerateWall({ 320,496 }, 640, 32);
	for (int i = 0; i < num; i++)
	{
		Vec2 pos = { 32 * rnd(0,19), 32 * rnd(0,14) };
		double width = 32 * rnd(2, 2)-2;
		double height = 32 * rnd(6, 6)-2;
		RectF rect = { Arg::center(pos), width, height };
		while (IsObstacle(rect))
		{
			pos = { 32 * rnd(0,19), 32 * rnd(0,14) };
			width = 32 * rnd(2, 2)-2;
			height = 32 * rnd(6, 6)-2;
			rect = { Arg::center(pos), width, height };
		}
		Actor* pWall = GenerateWall(pos, width, height);
		ActorManager::Get()->GetWalls().push_back(pWall);
	}
}

void Setup::GeneRandomMap2(int num)
{
	Actor* pWallT = GenerateWall({ -16,240 }, 32, 480);
	Actor* pWallB = GenerateWall({ 656,240 }, 32, 480);
	Actor* pWallL = GenerateWall({ 320,-16 }, 640, 32);
	Actor* pWallR = GenerateWall({ 320,496 }, 640, 32);
	for (int i = 0; i < num; i++)
	{
		Vec2 pos = { 32 * rnd(0,19), 32 * rnd(0,14) };
		double width = 32 * 2 - 4;
		double height = 32 * 2 - 4;
		RectF rect = { Arg::center(pos), width, height };
		while (IsObstacle(rect))
		{
			pos = { 32 * rnd(0,19), 32 * rnd(0,14) };
			width = 32 * 2 - 4;
			height = 32 * 2 - 4;
			rect = { Arg::center(pos), width, height };
		}
		Actor* pWall = GenerateWall(pos, width, height);
		ActorManager::Get()->GetWalls().push_back(pWall);
	}
}

void Setup::GeneDefaultMap()
{
	Actor* pWallT = GenerateWall({ -16,240 }, 32, 480);
	Actor* pWallB = GenerateWall({ 656,240 }, 32, 480);
	Actor* pWallL = GenerateWall({ 320,-16 }, 640, 32);
	Actor* pWallR = GenerateWall({ 320,496 }, 640, 32);

	
	Actor* pWall0 = GenerateWall({ 160,128 }, 128, 128);
	Actor* pWall1 = GenerateWall({ 160 +218,128 }, 128, 128);
	Actor* pWall2 = GenerateWall({ 160,128+218 }, 128, 128);
	Actor* pWall3 = GenerateWall({ 160 + 218,128 + 218 }, 256, 128);

	ActorManager::Get()->GetWalls().push_back(pWall0);
	ActorManager::Get()->GetWalls().push_back(pWall1);
	ActorManager::Get()->GetWalls().push_back(pWall2);
	ActorManager::Get()->GetWalls().push_back(pWall3);
}

void Setup::LoadMap()
{
	Actor* pWallT = GenerateWall({ -16,240 }, 32, 480);
	Actor* pWallB = GenerateWall({ 656,240 }, 32, 480);
	Actor* pWallL = GenerateWall({ 320,-16 }, 640, 32);
	Actor* pWallR = GenerateWall({ 320,496 }, 640, 32);

	stageMap_.LoadMap();
	auto& moveableMap = stageMap_.GetMapLayers()[MOVEABLE_STAGE1];
	for (int x = 0; x < stageMap_.GetWidth(); ++x)
	{
		for (int y = 0; y < stageMap_.GetHeight(); ++y)
		{			
			if (moveableMap[x + (y * stageMap_.GetWidth())] == -1)
			{
				Actor* pWall = GenerateWall(Common::NavMeshPosToPixelPos({ x,y }), Common::CHIPSIZE, Common::CHIPSIZE);
				ActorManager::Get()->GetWalls().push_back(pWall);
			}
		}
	}
}

void Setup::LoadStage1()
{
	Actor* pWallT = GenerateWall({ -16,240 }, 32, 480);
	Actor* pWallB = GenerateWall({ 656,240 }, 32, 480);
	Actor* pWallL = GenerateWall({ 320,-16 }, 640, 32);
	Actor* pWallR = GenerateWall({ 320,496 }, 640, 32);
	ActorManager::Get()->GetWalls().push_back(pWallT);
	ActorManager::Get()->GetWalls().push_back(pWallB);
	ActorManager::Get()->GetWalls().push_back(pWallL);
	ActorManager::Get()->GetWalls().push_back(pWallR);

	stageMap_.LoadMap();
	auto& moveableMap = stageMap_.GetMapLayers()[MOVEABLE_STAGE1];
	for (int x = 0; x < stageMap_.GetWidth(); ++x)
	{
		for (int y = 0; y < stageMap_.GetHeight(); ++y)
		{
			if (moveableMap[x + (y * stageMap_.GetWidth())] == -1)
			{
				double w = 0;
				double h = 0;
				if (moveableMap[x +1 + (y * stageMap_.GetWidth())] < 2)
				{
					w = 1;
				}
				else
				{
					w = moveableMap[x + 1 + (y * stageMap_.GetWidth())];
				}
				if (moveableMap[x + ((y + 1) * stageMap_.GetWidth())] < 2)
				{
					h = 1;
				}
				else
				{
					h = moveableMap[x + ((y + 1) * stageMap_.GetWidth())];
				}
				const int buffer2 = 28;//立体感を出すため、高さを削減
				Actor* pWall = GenerateWall({ (x + w / 2) * Common::CHIPSIZE, (y + h / 2) * Common::CHIPSIZE }, w * Common::CHIPSIZE, h * Common::CHIPSIZE - buffer2);
				ActorManager::Get()->GetWalls().push_back(pWall);
			}
		}
	}
}

void Setup::LoadStage2()
{
	Actor* pWallT = GenerateWall({ -16,240 }, 32, 480);
	Actor* pWallB = GenerateWall({ 656,240 }, 32, 480);
	Actor* pWallL = GenerateWall({ 320,-16 }, 640, 32);
	Actor* pWallR = GenerateWall({ 320,496 }, 640, 32);
	ActorManager::Get()->GetWalls().push_back(pWallT);
	ActorManager::Get()->GetWalls().push_back(pWallB);
	ActorManager::Get()->GetWalls().push_back(pWallL);
	ActorManager::Get()->GetWalls().push_back(pWallR);

	stageMap_.LoadMap();
	auto& moveableMap = stageMap_.GetMapLayers()[MOVEABLE_STAGE2];
	for (int x = 0; x < stageMap_.GetWidth(); ++x)
	{
		for (int y = 0; y < stageMap_.GetHeight(); ++y)
		{
			if (moveableMap[x + (y * stageMap_.GetWidth())] == -1)
			{
				double w = 0;
				double h = 0;
				if (moveableMap[x + 1 + (y * stageMap_.GetWidth())] < 2)
				{
					w = 1;
				}
				else
				{
					w = moveableMap[x + 1 + (y * stageMap_.GetWidth())];
				}
				if (moveableMap[x + ((y + 1) * stageMap_.GetWidth())] < 2)
				{
					h = 1;
				}
				else
				{
					h = moveableMap[x + ((y + 1) * stageMap_.GetWidth())];
				}
				const int buffer = 2;//壁に隙間を残す
				const int buffer2 = 28;//立体感を出すため、高さを削減
				Actor* pWall = GenerateWall({ (x + w / 2) * Common::CHIPSIZE, (y + h / 2) * Common::CHIPSIZE }, w * Common::CHIPSIZE - buffer, h * Common::CHIPSIZE - buffer2);
				ActorManager::Get()->GetWalls().push_back(pWall);
			}
		}
	}
}

void Setup::LoadStage3()
{
	Actor* pWallT = GenerateWall({ -16,240 }, 32, 480);
	Actor* pWallB = GenerateWall({ 656,240 }, 32, 480);
	Actor* pWallL = GenerateWall({ 320,-16 }, 640, 32);
	Actor* pWallR = GenerateWall({ 320,496 }, 640, 32);
	ActorManager::Get()->GetWalls().push_back(pWallT);
	ActorManager::Get()->GetWalls().push_back(pWallB);
	ActorManager::Get()->GetWalls().push_back(pWallL);
	ActorManager::Get()->GetWalls().push_back(pWallR);

	stageMap_.LoadMap();
	auto& moveableMap = stageMap_.GetMapLayers()[MOVEABLE_STAGE3];
	for (int x = 0; x < stageMap_.GetWidth(); ++x)
	{
		for (int y = 0; y < stageMap_.GetHeight(); ++y)
		{
			if (moveableMap[x + (y * stageMap_.GetWidth())] == -1)
			{
				double w = 0;
				double h = 0;
				if ( x == 19 || moveableMap[x + 1 + (y * stageMap_.GetWidth())] < 2)
				{
					w = 1;
				}
				else
				{
					w = moveableMap[x + 1 + (y * stageMap_.GetWidth())];
				}
				if (y == 14 || moveableMap[x + ((y + 1) * stageMap_.GetWidth())] < 2)
				{
					h = 1;
				}
				else
				{
					h = moveableMap[x + ((y + 1) * stageMap_.GetWidth())];
				}
				const int buffer = 2;//壁に隙間を残す
				const int buffer2 = 28;//立体感を出すため、高さを削減
				Actor* pWall = GenerateWall({ (x + w / 2) * Common::CHIPSIZE, (y + h / 2) * Common::CHIPSIZE }, w * Common::CHIPSIZE-buffer, h * Common::CHIPSIZE - buffer2);
				ActorManager::Get()->GetWalls().push_back(pWall);
			}
		}
	}
}

Enemy* Setup::GenerateEnemy()
{
	Enemy* pEnemy = DBG_NEW Enemy();
	CollisionComponent* pCollisionE = pEnemy->AddComponent<CollisionComponent, COLLISION>();
	pCollisionE->Edit().hitboxType = HitboxType::CIRCLE;
	pCollisionE->Edit().hitboxCircle = { pEnemy->GetPosition(), 16 };
	pCollisionE->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
	pCollisionE->Edit().collisionTo = Common::CollisionTag::COLLISION_ATTACKEFFECT;
	pCollisionE->Edit().isActivate = true;
	pCollisionE->Edit().hpEffect = Common::Enemy_Attack;
	pEnemy->SetHP(Common::EnemyMaxHP);	
	pEnemy->GetMovement().currentSpeed = rnd(150, 150);
	pEnemy->SetAttackId(1);
	pEnemy->SetTarget(pPlayer_);
	pEnemy->GetAgent().SetNavMesh(&navMesh_);	
	//StateController* pController = AIFactory::Get()->SetupStateMachineType2();
	StateController* pController = AIFactory::Get()->SetupStateMachineFromFile(U"FSMGraph/ghost.fsmg");
	pController->SetOwner(pEnemy);
	pController->SetOwnerEnemy(pEnemy);
	pEnemy->SetStateController(pController);	
	Vec2 playNavMeshPos = Common::PixelPosToNavMeshPos(pPlayer_->GetPosition());
	Point pt = { rnd(0,19), rnd(0,14) };
	const int distanceFromPlayer = 6;
	while (!navMesh_.isOnNavMesh(pt) || pt.distanceFromSq(playNavMeshPos) < distanceFromPlayer* distanceFromPlayer)
	{
		pt = { rnd(0,19), rnd(0,14) };
	}
	Vec2 pos = Common::NavMeshPosToPixelPos(pt);
	pEnemy->SetPosition(pos);
	if (isShowPathOn)
	{
		pEnemy->SetIsShowPathOn(true);
	}
	else
	{
		pEnemy->SetIsShowPathOn(false);
	}
	return pEnemy;
}

Enemy* Setup::GenerateEnemyType2()
{
	Enemy* pEnemy = DBG_NEW Enemy();
	CollisionComponent* pCollisionE = pEnemy->AddComponent<CollisionComponent, COLLISION>();
	pCollisionE->Edit().hitbox = { Arg::center(pEnemy->GetPosition()), 32, 32 };
	pCollisionE->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
	pCollisionE->Edit().collisionTo = Common::CollisionTag::COLLISION_ATTACKEFFECT;
	pCollisionE->Edit().isActivate = true;
	pCollisionE->Edit().hpEffect = Common::Enemy_Attack;
	pEnemy->SetHP(Common::EnemyMaxHP);
	pEnemy->GetMovement().currentSpeed = rnd(150, 150);
	pEnemy->SetAttackId(1);
	pEnemy->SetTarget(pPlayer_);
	pEnemy->GetAgent().SetNavMesh(&navMesh_);
	StateController* pController = AIFactory::Get()->SetupStateMachineType2();
	//StateController* pController = LevelManager::Get()->compiledController_;
	pController->SetOwner(pEnemy);
	pController->SetOwnerEnemy(pEnemy);
	pEnemy->SetStateController(pController);
	Vec2 pos = { rnd(16,624), rnd(16,464) };
	while (!navMesh_.isOnNavMesh(pos))
	{
		pos = { rnd(16,624), rnd(16,464) };
	}
	pEnemy->SetPosition(pos);
	if (isShowPathOn)
	{
		pEnemy->SetIsShowPathOn(true);
	}
	else
	{
		pEnemy->SetIsShowPathOn(false);
	}
	return pEnemy;
}

SkeletonKnight* Setup::GenerateSkeleton()
{
	SkeletonKnight* pSkeleton = DBG_NEW SkeletonKnight();
	CollisionComponent* pCollisionE = pSkeleton->AddComponent<CollisionComponent, COLLISION>();
	pCollisionE->Edit().hitboxType = HitboxType::CIRCLE;
	pCollisionE->Edit().hitboxCircle = { pSkeleton->GetPosition(), 16 };
	pCollisionE->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
	pCollisionE->Edit().collisionTo = Common::CollisionTag::COLLISION_ATTACKEFFECT;
	pCollisionE->Edit().isActivate = true;
	pCollisionE->Edit().hpEffect = Common::Enemy_Attack;
	pSkeleton->SetHP(Common::SkeletonMaxHP);
	pSkeleton->GetMovement().currentSpeed = rnd(150, 150);
	pSkeleton->SetAttackId(1);
	pSkeleton->SetTarget(pPlayer_);
	pSkeleton->GetAgent().SetNavMesh(&navMesh_);
	StateController* pController = AIFactory::Get()->SetupStateMachineSkeletonKnight();
	pController->SetOwner(pSkeleton);
	pController->SetOwnerEnemy(pSkeleton);
	pSkeleton->SetStateController(pController);
	pSkeleton->SetPosition({275,275});
	if (isShowPathOn)
	{
		pSkeleton->SetIsShowPathOn(true);
	}
	else
	{
		pSkeleton->SetIsShowPathOn(false);
	}
	return pSkeleton;
}

SkeletonKnightWhite* Setup::GenerateSkeletonWhite()
{
	SkeletonKnightWhite* pSkeleton = DBG_NEW SkeletonKnightWhite();
	CollisionComponent* pCollisionE = pSkeleton->AddComponent<CollisionComponent, COLLISION>();
	pCollisionE->Edit().hitboxType = HitboxType::CIRCLE;
	pCollisionE->Edit().hitboxCircle = { pSkeleton->GetPosition(), 16 };
	pCollisionE->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
	pCollisionE->Edit().collisionTo = Common::CollisionTag::COLLISION_ATTACKEFFECT;
	pCollisionE->Edit().isActivate = true;
	pCollisionE->Edit().hpEffect = Common::Enemy_Attack;
	pSkeleton->SetHP(Common::SkeletonMaxHP);
	pSkeleton->GetMovement().currentSpeed = rnd(150, 150);
	pSkeleton->SetAttackId(1);
	pSkeleton->SetTarget(pPlayer_);
	pSkeleton->GetAgent().SetNavMesh(&navMesh_);
	StateController* pController = AIFactory::Get()->SetupStateMachineSkeletonKnightWhite();
	pController->SetOwner(pSkeleton);
	pController->SetOwnerEnemy(pSkeleton);
	pSkeleton->SetStateController(pController);
	Vec2 playNavMeshPos = Common::PixelPosToNavMeshPos(pPlayer_->GetPosition());
	Point pt = { rnd(0,19), rnd(0,14) };
	const int distanceFromPlayer = 6;
	while (!navMesh_.isOnNavMesh(pt) || pt.distanceFromSq(playNavMeshPos) < distanceFromPlayer * distanceFromPlayer)
	{
		pt = { rnd(0,19), rnd(0,14) };
	}
	Vec2 pos = Common::NavMeshPosToPixelPos(pt);
	pSkeleton->SetPosition(pos);
	if (isShowPathOn)
	{
		pSkeleton->SetIsShowPathOn(true);
	}
	else
	{
		pSkeleton->SetIsShowPathOn(false);
	}
	return pSkeleton;
}


Knight* Setup::GeneratePlayer()
{
	Knight* pKnight = DBG_NEW Knight();
	pKnight->SetHP(Common::PlayerMaxHP);
	CollisionComponent* pCollisionK = pKnight->AddComponent<CollisionComponent, COLLISION>();
	pCollisionK->Edit().hitboxType = HitboxType::CIRCLE;
	pCollisionK->Edit().hitboxCircle = { pKnight->GetPosition(), Common::charRadius };
	pCollisionK->Edit().collisionFlags = Common::CollisionTag::COLLISION_KNIGHT;
	pCollisionK->Edit().collisionTo = Common::CollisionTag::COLLISION_ENEMY | Common::CollisionTag::COLLISION_HEALITEM;
	pCollisionK->Edit().isActivate = true;
	Vec2 pos = { rnd(16,624), rnd(16,464) };
	while (!navMesh_.isOnNavMesh(pos))
	{
		pos = { rnd(16,624), rnd(16,464) };
	}
	pKnight->SetPosition(pos);
	return pKnight;
}

HealItem* Setup::GenerateHealItem()
{
	HealItem* healItem = DBG_NEW HealItem;
	Point pt = { rnd(0,19), rnd(0,14) };
	while (!navMesh_.isOnNavMesh(pt))
	{
		pt = { rnd(0,19), rnd(0,14) };
	}
	Vec2 pos = Common::NavMeshPosToPixelPos(pt);
	healItem->SetPosition(pos);
	healItem->SetDesignatedPos(pos);
	return  healItem;
}
void Setup::PlayerInfinityHP()
{
	if (pPlayer_->GetHP() < Common::PlayerMaxHP)
	{
		pPlayer_->SetHP(Common::PlayerMaxHP);
	}	
}

void Setup::SetupGameLevel()
{	
	ActorManager::Get()->ClearActor();
	LoadStage1();
	navMesh_.AddObstacle();
	pPlayer_ = GeneratePlayer();
	pPlayer_->SetHP(Common::PlayerMaxHP);
	actorsNum_ = static_cast<int>(ActorManager::Get()->GetActors().size());
	GenerateEnemyType2();	
}

void Setup::SetupGameLevelStage1(const Vec2& i_startPos)
{
	ActorManager::Get()->ClearActor();
	LoadStage1();
	navMesh_.AddObstacle();
	pPlayer_ = GeneratePlayer();
	pPlayer_->SetHP(Common::PlayerMaxHP);
	pPlayer_->SetPosition(i_startPos);
	GenerateHealItem();
	GenerateHealItem();
	GenerateHealItem();
	actorsNum_ = static_cast<int>(ActorManager::Get()->GetActors().size());
	const int enemyNum = 5;
	for (int i= 0; i < enemyNum;++i)
	{
		GenerateEnemy();
	}
}
void Setup::SetupGameLevelStage2(const Vec2& i_startPos)
{
	ActorManager::Get()->ClearActor();
	LoadStage2();
	navMesh_.AddObstacle();
	pPlayer_ = GeneratePlayer();
	pPlayer_->SetPosition(i_startPos);
	GenerateHealItem();
	GenerateHealItem();
	GenerateHealItem();
	actorsNum_ = static_cast<int>(ActorManager::Get()->GetActors().size());
	const int enemyNum = 3;
	for (int i = 0; i < enemyNum; ++i)
	{
		GenerateEnemy();
	}
	const int skeletonNum = 2;
	for (int i = 0; i < skeletonNum; ++i)
	{
		GenerateSkeletonWhite();
	}
}
void Setup::SetupGameLevelStage3(const Vec2& i_startPos)
{
	ActorManager::Get()->ClearActor();
	LoadStage3();
	navMesh_.AddObstacle();
	pPlayer_ = GeneratePlayer();
	pPlayer_->SetPosition(i_startPos);
	GenerateHealItem();
	GenerateHealItem();
	GenerateHealItem();
	actorsNum_ = static_cast<int>(ActorManager::Get()->GetActors().size());
	Enemy* pEnemy1 = GenerateEnemy();
	Enemy* pEnemy2 = GenerateEnemy();
	Enemy* pEnemy3 = GenerateEnemy();
	pEnemy1->SetPosition({ 592,368 });
	pEnemy2->SetPosition({ 48,361 });
	pEnemy3->SetPosition({ 496,112 });
	Enemy* pEnemy4 = GenerateSkeleton();
	Enemy* pEnemy5 = GenerateSkeleton();
	pEnemy4->GetMovement().currentSpeed = 120;
	pEnemy5->GetMovement().currentSpeed = 180;
}




void Setup::Start()
{

}

void Setup::Update()
{	
	if (Key0.down())//マップクリア
	{
		ActorManager::Get()->ClearActor();
		stageMap_.ClearMap();
		navMesh_.AddObstacle();
		isPlayerInfinityHPOn = false;
		pPlayer_ = nullptr;
	}

	if (Key1.down())//基本ステージ生成
	{
		ActorManager::Get()->ClearActor();
		GeneDefaultMap();
		navMesh_.AddObstacle();
	}

	if (Key2.down())//ランダムで20個の壁を配置(正方形)
	{
		ActorManager::Get()->ClearActor();
		GeneRandomMap2(30);
		navMesh_.AddObstacle();
	}

	if (Key3.down())//ランダムで10個の壁を配置
	{
		ActorManager::Get()->ClearActor();
		GeneRandomMap(10);
		navMesh_.AddObstacle();
	}

	if (Key4.down())//プレイヤー生成
	{
		pPlayer_ = GeneratePlayer();
	}

	
	if (Key5.down())
	{
		if (pPlayer_ == nullptr)
		{
			Print(U"プレイヤーが存在しない");
		}
		else
		{			
			//GenerateEnemyFromGraph();
			GenerateEnemy();
		}
	}
	else if (KeyShift.pressed() && Key5.down())
	{
		if (pPlayer_ == nullptr)
		{
			Print(U"プレイヤーが存在しない");
		}
		else
		{
			GenerateSkeleton();
		}
	}
	else if (KeyControl.pressed() && Key5.down())//エネミー生成
	{
		if (pPlayer_ == nullptr)
		{
			Print(U"プレイヤーが存在しない");
		}
		else
		{
			for (int i = 0; i < 100; ++i)
			{
				GenerateEnemy();
			}
			//GenerateEnemy();
		}
	}


	if (Key6.down())//プレイヤーのHPを無限に
	{
		if (isPlayerInfinityHPOn)
		{
			isPlayerInfinityHPOn = false;
		}
		else
		{
			isPlayerInfinityHPOn = true;
		}		
	}
	if (isPlayerInfinityHPOn)
	{
		PlayerInfinityHP();
	}

	if (Key7.down())//ナビメッシュ表示On/Off
	{
		if (isNavMeshRenderOn)
		{
			isNavMeshRenderOn = false;
		}
		else
		{
			isNavMeshRenderOn = true;
		}
	}
	if (isNavMeshRenderOn)
	{
		navMesh_.RenderMesh();
	}	

	if (Key8.down())//経路の表示On/Off
	{
		if (isShowPathOn)
		{
			isShowPathOn = false;
			ActorManager::Get()->ShowPathOff();
		}
		else
		{
			isShowPathOn = true;
			ActorManager::Get()->ShowPathOn();
		}
	}
	if (KeyF3.down())//1マスずつマップの障害物生成
	{
		ActorManager::Get()->ClearActor();
		LoadMap();
		navMesh_.AddObstacle();
	}
	if (KeyF4.down())//障害物の大きさ情報から生成
	{
		ActorManager::Get()->ClearActor();
		LoadStage1();
		navMesh_.AddObstacle();
	}
	if (KeyF5.down())//壁コリジョンの描画On/Off
	{
		if (isDrawWallCollision)
		{
			isDrawWallCollision = false;
		}
		else
		{
			isDrawWallCollision = true;
		}
	}
	if (isDrawWallCollision)
	{
		//壁コリジョンの描画
		ActorManager::Get()->DrawWallCollision();
	}
	if (KeyF6.down())//障害物の大きさ情報から生成
	{
		ActorManager::Get()->ClearActor();
		LoadStage2();
		navMesh_.AddObstacle();
	}
	if (KeyF7.down())//障害物の大きさ情報から生成
	{
		ActorManager::Get()->ClearActor();
		LoadStage3();
		navMesh_.AddObstacle();		
	}
	if (KeyF11.down())
	{
		Print(pPlayer_->GetPosition());
	}
	if (KeyF10.down())
	{
		Vec2 pos = { 40,440 };
		pPlayer_->SetPosition(pos);
		pPlayer_->SetDesignatedPos(pos);
		//pPlayer_->SetPosition({ 395,180 });
		//pPlayer_->SetPosition({ 463,160 });
		//pPlayer_->SetKnockBackVec2({ rnd(-1,1),rnd(-1,1) });
		//pPlayer_->SetKnockBackVec2({ -1,0});
		//pPlayer_->SetHitting(true);
	}
	if(KeyZ.down())
	{
		pPlayer_->GetComponent<CollisionComponent, COLLISION>()->Edit().hitboxCircle.r += 1;
	}
	if (KeyX.down())
	{
		pPlayer_->GetComponent<CollisionComponent, COLLISION>()->Edit().hitboxCircle.r -= 1;		
	}
	if(KeyH.down())
	{
		for (int i = 0 ; i< 100 ; ++i)
		{
			GenerateHealItem();
		}
	}
	
}

void Setup::Remesh()
{
	navMesh_ = NaviMesh(Common::CHIPSIZE);
	SetNavMeshUnableOutDebugArea();
}

void Setup::SetNavMeshUnableOutDebugArea()
{
	const int mapWidth = navMesh_.GetMapWidth();
	const int mapHeight = navMesh_.GetMapHeight();
	auto& binaryMap = navMesh_.GetMap();
	//DebugAreaは画面の一部分しか使わない
	int minX = 0;
	int maxX = 17;
	int minY = 3;
	int maxY = 19;

	for (int x = 0; x < mapWidth; ++x)
	{
		for (int y = 0; y < mapHeight; ++y)
		{
			if (x < minX || x > maxX || y < minY || y > maxY)
			{
				binaryMap[x + (y * mapWidth)] = -1;
			}
			else
			{
				binaryMap[x + (y * mapWidth)] = 0;
			}
		}
	}
}

Enemy* Setup::GenerateEnemyFromGraph(StateController* pController)
{
	Enemy* pEnemy = DBG_NEW Enemy();
	CollisionComponent* pCollisionE = pEnemy->AddComponent<CollisionComponent, COLLISION>();
	pCollisionE->Edit().hitboxType = HitboxType::CIRCLE;
	pCollisionE->Edit().hitboxCircle = { pEnemy->GetPosition(), 16 };
	pCollisionE->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
	pCollisionE->Edit().collisionTo = Common::CollisionTag::COLLISION_ATTACKEFFECT;
	pCollisionE->Edit().isActivate = true;
	pCollisionE->Edit().hpEffect = Common::Enemy_Attack;
	pEnemy->SetHP(Common::EnemyMaxHP);
	pEnemy->GetMovement().currentSpeed = rnd(150, 150);
	pEnemy->SetAttackId(1);
	pEnemy->SetTarget(pPlayer_);
	pEnemy->GetAgent().SetNavMesh(&navMesh_);
	pController->SetOwner(pEnemy);
	pController->SetOwnerEnemy(pEnemy);
	pEnemy->SetStateController(pController);

	Vec2 genePos = { 300,200 };//TEST
	pEnemy->SetPosition(genePos);
	pEnemy->SetGoalPos(genePos);
	pEnemy->SetMoveable(false);
	if (isShowPathOn)
	{
		pEnemy->SetIsShowPathOn(true);
	}
	else
	{
		pEnemy->SetIsShowPathOn(false);
	}
	return pEnemy;
}

Enemy* Setup::SetupEditorLevelDebugArea(StateController* pController)
{
	ActorManager::Get()->ClearActor();
	Actor* pWallT = GenerateWall({ 288,80 }, 576, 32);
	Actor* pWallB = GenerateWall({ 288,656 }, 576, 32);
	Actor* pWallL = GenerateWall({ -16,363 }, 32, 576);
	Actor* pWallR = GenerateWall({ 592,363 }, 32, 576);
	ActorManager::Get()->GetWalls().push_back(pWallT);
	ActorManager::Get()->GetWalls().push_back(pWallB);
	ActorManager::Get()->GetWalls().push_back(pWallL);
	ActorManager::Get()->GetWalls().push_back(pWallR);
	pPlayer_ = GeneratePlayer();
	pPlayer_->SetHP(Common::PlayerMaxHP);
	return GenerateEnemyFromGraph(pController);
}

void Setup::SetupEditorLevelDebugArea()
{
	ActorManager::Get()->ClearActor();
	Actor* pWallT = GenerateWall({ 288,80 }, 576, 32);
	Actor* pWallB = GenerateWall({ 288,656 }, 576, 32);
	Actor* pWallL = GenerateWall({ -16,363 }, 32, 576);
	Actor* pWallR = GenerateWall({ 592,363 }, 32, 576);
	ActorManager::Get()->GetWalls().push_back(pWallT);
	ActorManager::Get()->GetWalls().push_back(pWallB);
	ActorManager::Get()->GetWalls().push_back(pWallL);
	ActorManager::Get()->GetWalls().push_back(pWallR);
}

Enemy* Setup::GenerateGhostWithoutAI()
{
	Enemy* pEnemy = DBG_NEW Enemy();
	CollisionComponent* pCollisionE = pEnemy->AddComponent<CollisionComponent, COLLISION>();
	pCollisionE->Edit().hitboxType = HitboxType::CIRCLE;
	pCollisionE->Edit().hitboxCircle = { pEnemy->GetPosition(), 16 };
	pCollisionE->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
	pCollisionE->Edit().collisionTo = Common::CollisionTag::COLLISION_ATTACKEFFECT;
	pCollisionE->Edit().isActivate = true;
	pCollisionE->Edit().hpEffect = Common::Enemy_Attack;
	pEnemy->SetHP(Common::EnemyMaxHP);
	pEnemy->GetMovement().currentSpeed = rnd(150, 150);
	pEnemy->SetAttackId(1);
	pEnemy->GetAgent().SetNavMesh(&navMesh_);

	Vec2 genePos = { 300,200 };
	pEnemy->SetPosition(genePos);
	pEnemy->SetGoalPos(genePos);
	pEnemy->SetTargetPos(genePos);
	pEnemy->SetMoveable(false);	
	return pEnemy;
}

SkeletonKnight* Setup::GenerateSkeletonWithoutAI()
{
	SkeletonKnight* pSkeleton = DBG_NEW SkeletonKnight();
	CollisionComponent* pCollisionE = pSkeleton->AddComponent<CollisionComponent, COLLISION>();
	pCollisionE->Edit().hitboxType = HitboxType::CIRCLE;
	pCollisionE->Edit().hitboxCircle = { pSkeleton->GetPosition(), 16 };
	pCollisionE->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
	pCollisionE->Edit().collisionTo = Common::CollisionTag::COLLISION_ATTACKEFFECT;
	pCollisionE->Edit().isActivate = true;
	pCollisionE->Edit().hpEffect = Common::Enemy_Attack;
	pSkeleton->SetHP(Common::SkeletonMaxHP);
	pSkeleton->GetMovement().currentSpeed = rnd(150, 150);
	pSkeleton->SetAttackId(1);
	pSkeleton->GetAgent().SetNavMesh(&navMesh_);
	pSkeleton->SetPosition({ 275,275 });

	Vec2 genePos = { 300,200 };
	pSkeleton->SetPosition(genePos);
	pSkeleton->SetGoalPos(genePos);
	pSkeleton->SetTargetPos(genePos);
	pSkeleton->SetMoveable(false);
	return pSkeleton;
}

SkeletonKnightWhite* Setup::GenerateSkeletonWhiteWithoutAI()
{
	SkeletonKnightWhite* pSkeleton = DBG_NEW SkeletonKnightWhite();
	CollisionComponent* pCollisionE = pSkeleton->AddComponent<CollisionComponent, COLLISION>();
	pCollisionE->Edit().hitboxType = HitboxType::CIRCLE;
	pCollisionE->Edit().hitboxCircle = { pSkeleton->GetPosition(), 16 };
	pCollisionE->Edit().collisionFlags = Common::CollisionTag::COLLISION_ENEMY;
	pCollisionE->Edit().collisionTo = Common::CollisionTag::COLLISION_ATTACKEFFECT;
	pCollisionE->Edit().isActivate = true;
	pCollisionE->Edit().hpEffect = Common::Enemy_Attack;
	pSkeleton->SetHP(Common::SkeletonMaxHP);
	pSkeleton->GetMovement().currentSpeed = rnd(150, 150);
	pSkeleton->SetAttackId(1);
	pSkeleton->GetAgent().SetNavMesh(&navMesh_);
	pSkeleton->SetPosition({ 275,275 });

	Vec2 genePos = { 300,200 };
	pSkeleton->SetPosition(genePos);
	pSkeleton->SetGoalPos(genePos);
	pSkeleton->SetTargetPos(genePos);
	pSkeleton->SetMoveable(false);
	return pSkeleton;
}
void Setup::StartDebug(Enemy* i_pEnemy)
{
	if(!pPlayer_)
	{
		pPlayer_ = GeneratePlayer();
		pPlayer_->SetHP(Common::PlayerMaxHP);
		pPlayer_->SetPosition({ 287,452 });
	}
	i_pEnemy->SetTarget(pPlayer_);
}

void Setup::StopDebug()
{
	ActorManager::Get()->ClearActor();
	pPlayer_ = nullptr;
}


void Setup::SetupAI(Enemy* i_pEnemy, StateController* i_pController)
{
	StateController* forDelete = i_pEnemy->GetStateController();
	if (forDelete)
	{
		delete forDelete;
	}
	i_pController->SetOwner(i_pEnemy);
	i_pController->SetOwnerEnemy(i_pEnemy);
	i_pEnemy->SetStateController(i_pController);
}





