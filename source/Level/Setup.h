#pragma once
#include "Actor/Actor.h"
#include "Actor/Wall.h"
#include "Core/Random.h"
#include "Actor/ActorManager.h"
#include "Navigation/NaviMesh.h"
#include "Actor/Enemy.h"
#include "Navigation/NaviMesh.h"
#include "AIGraph/StateMachine/AIFactory.h"
#include "Actor/Knight.h"
#include "Actor/SkeletonKnight.h"
#include "Level/StageMap.h"
#include "MapItem/HealItem.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

class Setup
{
public:
	Setup() {}
	~Setup() = default;
	//プレイヤーキャラクターを取得
	const Knight& GetPlayer() const { return *pPlayer_; }
	//プレイヤーキャラクターを取得(非const)
	Knight& GetPlayer() { return *pPlayer_; }

	//ゲーム開始時敵以外のアクター数を取得
	const int GetActorsNum() const { return actorsNum_; }
	StageMap& GetMap() { return stageMap_; }
	NaviMesh& GetNavMesh() { return navMesh_; }
	//壁の生成
	Actor* GenerateWall(const Vec2& i_pos, double i_w, double i_h);
	//ランダムで壁の生成
	void GeneRandomMap(int num);
	//ランダムで壁の生成
	void GeneRandomMap2(int num);
	void GeneDefaultMap();
	void SetupGameLevel();

	//ゲーム本体用
	void SetupGameLevelStage1(const Vec2& i_startPos);
	void SetupGameLevelStage2(const Vec2& i_startPos);
	void SetupGameLevelStage3(const Vec2& i_startPos);

	//1マスずつマップの障害物生成
	void LoadMap();
	//障害物の大きさ情報から生成
	void LoadStage1();
	void LoadStage2();
	void LoadStage3();
	//エネミーの生成
	Enemy* GenerateEnemy();
	Enemy* GenerateEnemyType2();
	SkeletonKnight* GenerateSkeleton();
	Knight* GeneratePlayer();
	HealItem* GenerateHealItem();
	void PlayerInfinityHP();
	//ゲーム開始時のセットアップ
	void Start();
	//ボタン押下で機能が使える
	void Update();

	//AI Editor用
	void Remesh();
	void SetNavMeshUnableOutDebugArea();
	Enemy* GenerateEnemyFromGraph(StateController* pController);
	Enemy* SetupEditorLevelDebugArea(StateController* pController);
	void SetupEditorLevelDebugArea();
	Enemy* GenerateGhostWithoutAI();
	SkeletonKnight* GenerateSkeletonWithoutAI();
	void StartDebug(Enemy* i_pEnemy);
	void StopDebug();
	void SetupAI(Enemy* i_pEnemy, StateController* i_pController);



private:
	NaviMesh navMesh_ = NaviMesh(Common::CHIPSIZE);
	//AIFactory aiFactory_;
	StageMap stageMap_;
	Knight* pPlayer_ = nullptr;
	int actorsNum_ = 0;
	bool isPlayerInfinityHPOn = false;
	bool isNavMeshRenderOn = false;
	bool isShowPathOn = false;
	bool isDrawWallCollision = false;
};
