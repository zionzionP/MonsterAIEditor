#pragma once
#include <Siv3D.hpp>
#include <vector>
#include "Core/Singleton.h"
#include "Actor/Actor.h"
#include "Component/CollisionComponent.h"

class Actor;
//インスタンス取得: ActorManager::Get()
class ActorManager : public Singleton<ActorManager>
{
public:
	ActorManager();
	~ActorManager() = default;
	typedef std::vector<Actor*> ActorList;
	//アクターリストの取得
	const ActorList& GetActors() const { return actors_; }
	//アクターリストの取得(非const)
	ActorList& GetActors() { return actors_; }
	//壁リストの取得
	const ActorList& GetWalls() const { return wallList_; }
	//壁リストの取得
	ActorList& GetWalls(){ return wallList_; }
	//敵リストの取得
	const ActorList& GetEnemies() const { return enemies_; }
	//敵リストの取得
	ActorList& GetEnemies() { return enemies_; }
	//アクターの追加(アクターのコンストラクタに呼び出せる)
	void AddActor(Actor* i_pActor);
	//アクターの削除(アクターのデストラクタに呼び出せる)
	void RemoveActor(Actor* i_pActor);
	//アクターの描画以外の更新
	void Update(double i_deltaTime);

	void UpdatePosition();
	//アクターの描画の更新
	void UpdateRender(uint64 i_millisecTime);
	//壁リストの生成
	void GeneWallList();
	//壁のコリジョンを描画
	void DrawWallCollision();
	//アクター全クリア
	void ClearActor();
	//経路表示On/Off
	void ShowPathOn();
	void ShowPathOff();
	//アクターに付属のEffectを一斉に更新
	void UpdateEffect();
private:
	std::vector<Actor*> actors_;
	std::vector<Actor*> wallList_;
	std::vector<Actor*> enemies_;
	
};
