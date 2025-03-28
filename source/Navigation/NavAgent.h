#pragma once
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include "Navigation/NaviMesh.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

class Enemy;
//経路探索のノード
struct Node
{
public:
	Vec2 position;
	Node* parentNode;

	//移動コスト
	float moveCost;
	//距離コスト
	float distance;
	//合計コスト
	float totalCost;

	Node() = delete;
	Node(const Vec2& i_pos, Node* i_parent, float i_moveCost, float i_distance, float i_totalCost)
		:position(i_pos), parentNode(i_parent), moveCost(i_moveCost), distance(i_distance), totalCost(i_totalCost)
	{}
	~Node() = default;

	bool operator==(const Node& other) const { return position == other.position; }
	bool operator==(const Vec2& other) const { return position == other; }

	bool operator< (const Node& other)const { return totalCost - other.totalCost < -std::numeric_limits<float>::epsilon(); }


	bool operator> (const Node& other)const { return totalCost - other.totalCost > std::numeric_limits<float>::epsilon(); }
};

class NavAgent
{
public:
	NavAgent() {}
	~NavAgent() = default;
	//ナビメッシュを取得
	const NaviMesh& GetNavMesh() const { return *pNavMesh; }
	//ナビメッシュをセット
	void SetNavMesh(NaviMesh* i_pmesh) { pNavMesh = i_pmesh; }
	//A*アルゴリズムで経路探索
	void FindPath(const Vec2& i_start, const Vec2& i_goal);
	//経路の表示
	void ShowPath()const;
	//経路の最適化
	void OptimizePath();
private:
	friend Enemy;
	NaviMesh* pNavMesh = nullptr;
	std::vector<Vec2> path_;
	//4方向の経路探索
	Vec2 adjacent[4] = { Vec2(0, -1), Vec2(-1, 0),Vec2(0, 1), Vec2(1, 0)
						/*,Vec2(-1, -1),Vec2(1, -1),Vec2(-1, 1),Vec2(1, 1)*/  };
};
