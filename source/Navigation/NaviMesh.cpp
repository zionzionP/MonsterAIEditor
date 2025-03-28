#include "Navigation/NaviMesh.h"

NaviMesh::NaviMesh(int i_length) :
	chipSize_(i_length),
	mapWidth_(Scene::Size().x / i_length),
	mapHeight_(Scene::Size().y / i_length),
	binaryMap_(std::vector<int>(mapWidth_ * mapHeight_, 0))
{
}

bool NaviMesh::IsObstacle(const Vec2& i_pos)
{
	RectF rect = { i_pos, chipSize_, chipSize_ };
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

void NaviMesh::AddObstacle()
{
	for (int x = 0; x < mapWidth_; ++x)
	{
		for (int y = 0; y < mapHeight_; ++y)
		{
			if (IsObstacle({ x * chipSize_, y * chipSize_ }))
			{
				binaryMap_[x + (y * mapWidth_)] = -1;
			}
			else
			{
				binaryMap_[x + (y * mapWidth_)] = 0;
			}
		}
	}
}

void NaviMesh::RenderMesh()
{
	for (int x = 0; x < mapWidth_; ++x)
	{
		for (int y = 0; y < mapHeight_; ++y)
		{
			if (binaryMap_[x + (y * mapWidth_)] == 0)
			{
				RectF rect = { { x * chipSize_, y * chipSize_ }, chipSize_, chipSize_ };
				rect.draw(Palette::Lightgreen);
				rect.drawFrame(1, Palette::Yellowgreen);
			}
		}
	}
}

bool NaviMesh::isOnNavMesh(const Vec2& i_vec2) const 
{
	Vec2 meshPos = Common::PixelPosToNavMeshPos(i_vec2);
	if (meshPos.x < 0 || meshPos.x > mapWidth_-1 || meshPos.y < 0 || meshPos.y > mapHeight_- 1)
	{
		return false;
	}
	if (binaryMap_[meshPos.x + (meshPos.y * mapWidth_)] == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool NaviMesh::isOnNavMesh(const Point& i_point) const
{
	if (i_point.x < 0 || i_point.x > mapWidth_ - 1 || i_point.y < 0 || i_point.y > mapHeight_ - 1)
	{
		return false;
	}
	if (binaryMap_[i_point.x + (i_point.y * mapWidth_)] == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}



