#include "Navigation/NavAgent.h"
void NavAgent::FindPath(const Vec2& i_start, const Vec2& i_goal)
{
	// OpenList: 探索予定のノードを保存
	std::priority_queue < Node*, std::vector<Node*>, decltype([](const Node* a, const Node* b) { return (*a) > (*b); }) > openList;

	// ClosedList: 探索済みのノードを保存、アルゴリズム終了時delete
	std::vector<Node*> closedList;
	closedList.reserve(300);

	// OpenSet: 探索予定のノードの位置を保存
	std::set<int> openSet;

	// ClosedSet: 探索済みのノードの位置を保存
	std::set<int> closedSet;

	// 追跡用の全ノードリスト（リーク防止）
	std::vector<Node*> allNodes;

	auto& map = pNavMesh->binaryMap_;
	int width = pNavMesh->mapWidth_;
	int height = pNavMesh->mapHeight_;
	Node* pStartNode = DBG_NEW Node(i_start, nullptr, 0.f, 0.f, 0.f);
	allNodes.push_back(pStartNode);

	openList.push(pStartNode);
	openSet.insert(pStartNode->position.x + pStartNode->position.y * width);

	while (!openList.empty())
	{
		// OpenListの中に一番コスト低いノードを現在のノードに設定、OpenListから削除し、その位置をCloseListに保存
		Node* pCurrentNode = openList.top();
		openList.pop();
		closedList.push_back(pCurrentNode);
		openSet.erase(pCurrentNode->position.x + pCurrentNode->position.y * width);
		closedSet.insert(pCurrentNode->position.x + pCurrentNode->position.y * width);

		// 目的地に到達した場合
		if (pCurrentNode->position == i_goal)
		{
			while (pCurrentNode != nullptr)
			{
				path_.insert(path_.begin(), pCurrentNode->position);
				pCurrentNode = pCurrentNode->parentNode;
			}
			OptimizePath();

			// すべてのノードを解放
			for (Node* pNode : allNodes)
			{
				delete pNode;
			}
			return;
		}

		std::vector<Node*> childrenList;
		// 周囲マスに子ノードを生成、親ノードは現在のノード
		for (const auto& adjacentPos : adjacent)
		{
			Vec2 childPos = adjacentPos + pCurrentNode->position;
			if (childPos.x >= 0 && childPos.y >= 0 && childPos.x < width && childPos.y < height)
			{
				if (map[static_cast<int>(childPos.x) + (static_cast<int>(childPos.y) * width)] == 0)
				{
					Node* pChild = DBG_NEW Node(childPos, pCurrentNode, adjacentPos.length(), 0.f, 0.f);
					childrenList.push_back(pChild);
					allNodes.push_back(pChild);
				}
			}
		}

		// 子ノードのコスト計算、探索されたことない子ノードをOpenListに保存
		for (Node* pChild : childrenList)
		{
			int childHash = pChild->position.x + pChild->position.y * width;
			if (closedSet.find(childHash) == closedSet.end())
			{
				pChild->moveCost += pCurrentNode->moveCost;
				pChild->distance = pChild->position.distanceFromSq(i_goal) / ((height - 1) * (width - 1));
				pChild->totalCost = pChild->moveCost + pChild->distance;

				if (openSet.find(childHash) == openSet.end())
				{
					openList.push(pChild);
					openSet.insert(childHash);
				}
			}
		}
	}

	//Print << U"到達不可";

	// すべてのノードを解放
	for (Node* pNode : allNodes)
	{
		delete pNode;
	}
}
# if 0//旧バージョン
void NavAgent::FindPath(const Vec2& i_start, const Vec2& i_goal)
{
	//OpenList:探索予定のノードを保存
	std::priority_queue < Node*, std::vector<Node*>, decltype([](const Node* a, const Node* b) { return (*a) > (*b); }) > openList;
	//ClosedList:探索済みのノードを保存、アルゴリズム終了時delete
	std::vector<Node*> closedList;
	closedList.reserve(300);
	//OpenSet:探索予定のノードの位置を保存
	std::set<int> openSet;
	//OpenSet:探索済みのノードの位置を保存
	std::set<int> closedSet;
	
	auto& map = pNavMesh->binaryMap_;
	int width = pNavMesh->mapWidth_;
	int height = pNavMesh->mapHeight_;
	Node* pStartNode = DBG_NEW Node(i_start, nullptr, 0.f, 0.f, 0.f);


	openList.push(pStartNode);
	openSet.insert(pStartNode->position.x + pStartNode->position.y * width);

	while (openList.size() > 0)
	{
		//OpenListの中に一番コスト低いノードを現在のノードに設定、OpenListから削除し、その位置をCloseListに保存
		Node* pCurrentNode = openList.top();
		openList.pop();
		closedList.push_back(pCurrentNode);
		auto itr = openSet.erase(pCurrentNode->position.x + pCurrentNode->position.y * width);
		closedSet.insert(pCurrentNode->position.x + pCurrentNode->position.y * width);

        //現在のノードはゴール地点の場合、探索終了。パスを保存
		if (pCurrentNode->position == i_goal)
		{
			while (pCurrentNode != nullptr)
			{
				path_.insert(path_.begin(), pCurrentNode->position);
				pCurrentNode = pCurrentNode->parentNode;
			}
			while (!openList.empty())
			{
				Node* pForDelete = openList.top();
				openList.pop();
				delete pForDelete;
			}
			for (Node* pNode : closedList)
			{
				delete pNode;
			}
			OptimizePath();
			return;
		}

		std::vector<Node*> childrenList;
		//周囲マスに子ノードを生成、親ノードは現在のノード
		for (const auto& adjacentPos : adjacent)
		{
			Vec2 childPos = adjacentPos + pCurrentNode->position;
			if (childPos.x >= 0 && childPos.y >= 0 && childPos.x < width && childPos.y < height)
			{
				if (map[static_cast<int>(childPos.x) + (static_cast<int>(childPos.y) * width)] == 0)
				{
					childrenList.emplace_back(DBG_NEW Node(childPos, pCurrentNode, adjacentPos.length(), 0.f, 0.f));
				}
			}
		}
		//子ノードのコスト計算、探索されたことない子ノードをOpenListに保存
		for (Node* pChild : childrenList)
		{
			if (closedSet.find(pChild->position.x + pChild->position.y * width) == closedSet.end())
			{
				pChild->moveCost += pCurrentNode->moveCost;
				pChild->distance = pChild->position.distanceFromSq(i_goal) / ((height-1)*(width-1));
				pChild->totalCost = pChild->moveCost + pChild->distance;

				if (openSet.find(pChild->position.x + pChild->position.y * width) == openSet.end())
				{
					openList.push(pChild);
					openSet.insert(pChild->position.x + pChild->position.y * width);
					continue;
				}
			}
			delete pChild;
		}
	}
	Print << U"到達不可";
}
#endif


//線は障害物の対角線に経過しているかの判断
bool IsObstacle(const Line& i_line)
{
	ActorManager* pManager = ActorManager::Get();
	for (auto& pActor : pManager->GetWalls())
	{
		CollisionComponent* pCollision = pActor->GetComponent<CollisionComponent, COLLISION>();	
		if (pCollision != nullptr)
		{
			Line diagonal1{ {pCollision->Get().hitbox.bl().x - Common::CHIPSIZE / 2,pCollision->Get().hitbox.bl().y + Common::CHIPSIZE / 2},
						    {pCollision->Get().hitbox.tr().x + Common::CHIPSIZE / 2,pCollision->Get().hitbox.tr().y - Common::CHIPSIZE / 2} };
			Line diagonal2{ {pCollision->Get().hitbox.br().x + Common::CHIPSIZE / 2,pCollision->Get().hitbox.br().y + Common::CHIPSIZE / 2},
							{pCollision->Get().hitbox.tl().x - Common::CHIPSIZE / 2,pCollision->Get().hitbox.tl().y - Common::CHIPSIZE / 2} };
			if (diagonal1.intersects(i_line) || diagonal2.intersects(i_line))
			{
				return true;
			}
		}
	}
	return false;
}

//パスの最適化
void NavAgent::OptimizePath()
{
	for (auto pos1 = path_.begin(); pos1 != path_.end(); pos1++)
	{
		for (auto pos2 = pos1 + 1; pos2 != path_.end(); )
		{
			//パス上の2点で線を引く
			Line line{ Common::NavMeshPosToPixelPos(*pos1), Common::NavMeshPosToPixelPos(*pos2) };
			//障害物経過の判断
			if (IsObstacle(line))
			{
				if(pos2-1 >= pos1+ 1)  //経過の場合、中間地点を削除
				{
					pos1 = path_.erase(pos1 + 1, pos2 - 1);
				}
				break;				
			}
			else
			{
				pos2++;
			}
			if (pos2 == path_.end())  //最後まで交差しながったら、直接最後の目標位置に向かう
			{
				pos1 = path_.erase(pos1+1, pos2-1);
				return;
			}
		}
	}
}

void NavAgent::ShowPath()const
{
	for (const auto& pos : path_)
	{
		RectF rect = { { pos.x * Common::CHIPSIZE, pos.y * Common::CHIPSIZE }, Common::CHIPSIZE, Common::CHIPSIZE };
		rect.draw(Palette::Red);
	}
}


