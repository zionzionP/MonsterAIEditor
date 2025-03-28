#pragma once
#include <Siv3D.hpp>
#include "Actor/ActorManager.h"
#include "Component/CollisionComponent.h"
#include <vector>

class NavAgent;

class NaviMesh
{
public:
	NaviMesh() {};
	NaviMesh(int i_length);
	~NaviMesh() = default;

	NaviMesh(const NaviMesh& i_navMesh) = default;

	//NavMeshを取得
	const auto& GetMap() const { return binaryMap_; }
	//NavMeshを取得（非const）
	auto& GetMap() { return binaryMap_; }
	int GetChipSize() const { return chipSize_; }
	int GetMapWidth() const { return mapWidth_; }
	int GetMapHeight() const { return mapHeight_; }

	//ナビメッシュに障害物の位置を設置
	void AddObstacle();
	//設置中、当マスに障害物はあるかを判断
	bool IsObstacle(const Vec2& i_pos);
	//メッシュの描画
	void RenderMesh();
	//positionはナビメッシュ上にあるかを判断
	bool isOnNavMesh(const Vec2& i_vec2) const ;
	//positionはナビメッシュ上にあるかを判断
	bool isOnNavMesh(const Point& i_point) const;
	

private:
	friend NavAgent;
	//1マスの辺の長さ
	int chipSize_ = 16;
	//幅
	int mapWidth_ = 40;
	//高さ
	int mapHeight_ = 30;
	//マップ、配列は定数でしか初期化できないためvectorを採用
	std::vector<int> binaryMap_;
};


