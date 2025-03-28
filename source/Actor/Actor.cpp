#include "Actor/Actor.h"
Actor::Actor():
	isDead_(false),
	isMoveable_  (true),
	isMoving_ (false),
	isHitting_ (false),
    position_  { 0, 0 },
	HP_(100)
{
	ActorManager::Get()->AddActor(this);
}

Actor::~Actor()
{
	ComponentFactoryManager* pManager = ComponentFactoryManager::Get();
	for (auto& component : components_)
	{
		pManager->RemoveComponent(component.first, component.second);
	}	
}

void Actor::UpdateComponentReference(int i_type, int i_index)
{
	auto itr = components_.find(i_type);
	if (itr != components_.end())
	{
		itr->second = i_index;
	}
}

void Actor::AddDeltaPos()
{
	Vec2 allDeltaPos{ 0,0 };
	for (const auto& deltaPos : DeltaPositions_)
	{
		allDeltaPos += deltaPos;
	}
	designatedPos_ += allDeltaPos;
}

//止める位置を返すだけのスイープ
const BlockingHitPoint Actor::Sweep(const Vec2& i_start, const Vec2& i_remainVelocity)
{
	//判定とスタート位置が重ね合わせ防止のバッファー
	const double buffer = 0.01*1.414 ;
	//スタート地点に移動ベクトル逆方向にある仮想のスタート地点
	Vec2 virtualStart = i_start - buffer * i_remainVelocity/i_remainVelocity.length();
	Vec2 end = i_start + i_remainVelocity;
	Line sweepLine{ virtualStart,end };

	std::vector<BlockingHitPoint> blockingHits;
	ActorManager* pManager = ActorManager::Get();
	auto thisCollision = this->GetComponent<CollisionComponent, COLLISION>();
	
	const double charRadius = thisCollision->Get().hitboxCircle.r;
	for (auto pActor : pManager->GetWalls())
	{
		CollisionComponent* pCollision = pActor->GetComponent<CollisionComponent, COLLISION>();
		if (pCollision != nullptr)
		{
			const RectF& hitbox = pCollision->Get().hitbox;
			Vec2 closestPointToHitbox = Common::ClosestPointAABB(i_start, hitbox);
			Vec2 vec2ToClosestPoint = closestPointToHitbox - i_start;
			//運動ベクトルと同じ方向だけ処理する
			if (Dot(i_remainVelocity, vec2ToClosestPoint) / (i_remainVelocity.length() * vec2ToClosestPoint.length()) <= DBL_EPSILON)
			{
				continue;
			}
			//長方形の周りにキャラクター半径を追加、移動のベクトルと交差判定
			RoundRect hitboxr{ Arg::center(hitbox.center()), hitbox.w + 2 * charRadius,hitbox.h + 2 * charRadius,charRadius };
			//hitboxr.drawFrame(1);
			auto points = hitboxr.intersectsAt(sweepLine);
			if (points != std::nullopt)
			{
				for (const auto& point : points.value())
				{
					//交差点を保存
					blockingHits.emplace_back(point, pCollision);
				}
			}
		}
	}
	//ヒットしない場合、endを返す
	if (blockingHits.empty())
	{
		return BlockingHitPoint(end,nullptr);
	}
	else
	{
		//一番近いヒットの場所を選択
		double minDistance = DBL_MAX;
		int minIndex = -1;
		for (int i = 0; i < blockingHits.size(); ++i)
		{
			double distance = i_start.distanceFrom(blockingHits[i].hitPoint);
			if (distance < minDistance)
			{
				minDistance = distance;
				minIndex = i;
			}
		}
		return blockingHits[minIndex];
	}
}

void Actor::UpdateMovement()
{
	//スイープスタート地点
	Vec2 start = position_;
	//残りの移動量
	Vec2 remainDeltaPos = deltaPos_;
	Vec2 end = start + remainDeltaPos;
	//スイープする閾値
	double sweepThreshold = 0.01;
	//残り移動量が閾値以下になるまでスイープする
	while (remainDeltaPos.lengthSq() > pow(sweepThreshold, 2))
	{
		BlockingHitPoint hitPosition = Sweep(start, remainDeltaPos);
		if(hitPosition.hitCollision == nullptr)
		{
			end = hitPosition.hitPoint;
			break;
		}
		else
		{
			Vec2 closestPoint = Common::ClosestPointAABB(hitPosition.hitPoint, hitPosition.hitCollision->Get().hitbox);
			Vec2 vecToClosestPoint = closestPoint - hitPosition.hitPoint;
			Vec2 newRemainDeltaPos = start + remainDeltaPos - hitPosition.hitPoint;

			if (Dot(newRemainDeltaPos, vecToClosestPoint) > 0)
			{
				//接触面と垂直になる移動ベクトルだけブロック
				Vec2 projection = vecToClosestPoint * Dot(newRemainDeltaPos, vecToClosestPoint) / vecToClosestPoint.lengthSq();
				newRemainDeltaPos = newRemainDeltaPos - projection;
			}
			start = hitPosition.hitPoint;
			remainDeltaPos = newRemainDeltaPos;
			end = start + remainDeltaPos;
		}
		
	}
	designatedPos_ = end;
}



