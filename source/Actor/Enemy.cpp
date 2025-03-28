#include "Actor/Enemy.h"

#include "Actor/SkeletonKnight.h"

void Enemy::OnHitted(int i_collisionTag,const Actor* i_pOther, int i_hpEffect, int i_attackId)
{
	if (i_collisionTag == Common::CollisionTag::COLLISION_ATTACKEFFECT)
	{
		Actor* pOther = const_cast<Actor*>(i_pOther);
		Actor* pAttackOwner = static_cast<Attack*>(pOther)->GetOwner();
		Line line{ pAttackOwner->GetDesignatedPos(), designatedPos_ };
		for (auto pActor : ActorManager::Get()->GetWalls())
		{
			CollisionComponent* pCollision = pActor->GetComponent<CollisionComponent, COLLISION>();
			if (pCollision != nullptr)
			{
				if (pCollision->Get().hitbox.intersects(line))
				{
					return ;
				}
			}
		}
	}
	if (lastAttackId_ < i_attackId)
	{
		lastAttackId_ = i_attackId;
		HP_ = HP_ + i_hpEffect;	
		Vec2 diff = designatedPos_ - i_pOther->GetDesignatedPos();//位置差分をとる
		if (diff.lengthSq() < FLT_EPSILON)//位置が重なった場合、位置差分を右に1マスにする
		{
			diff = { 32,32 };
		}
		knockBackVec2_ = diff / diff.length();//ノックバックの方向設定
		effect_.add<HitEffectSword>(position_);
		isHitting_ = true;
		atkTimer_ = 0;
	}
}
//Collisionであたりを判定し、判定発生後の処理
void Enemy::UpdateHitting(double i_deltaTime)
{
	if (isHitting_)
	{
		isMoveable_ = false;
		isAttacking_ = false;
		if (KnockBackDone(i_deltaTime))
		{
			isDamageable_ = true;
			isHitting_ = false;
			isMoveable_ = true;
		}
	}
}

//ノックバック処理
bool Enemy::KnockBackDone(double i_deltaTime)
{
	KBTimer += i_deltaTime;
	if (KBTimer < 0.1)
	{
		//ノックバック時間(0.1)秒内ノックバック距離(100)を移動 
		deltaPos_ += knockBackVec2_ * Common::KNOCKBACK_DISTANCE * i_deltaTime / 0.1;
		return false;
	}
	KBTimer = 0.0;
	return true;
}

void Enemy::UpdateAttack(double i_deltaTime)
{
	if (isAttacking_)
	{
		isMoveable_ = false;
		if (AttackDone(i_deltaTime))
		{			
			isAttacking_ = false;
			isMoveable_ = true;
		}
	}
}
bool Enemy::AttackDone(double i_deltaTime)
{
	const double chargeTime = 0.4;
	const double tackleTime = 0.1;
	const double backTobeforePosTime = 0.2;
	const double chargeDistance = 0.2 * Common::TACKLE_DISTANCE;
	atkTimer_ += i_deltaTime;
	if (atkTimer_ < chargeTime)
	{
		deltaPos_ -= attackVec2_ * chargeDistance * i_deltaTime / chargeTime;
		return false;
	}
	else if (atkTimer_ < chargeTime+tackleTime)
	{
		deltaPos_ += attackVec2_ * (Common::TACKLE_DISTANCE+chargeDistance) * i_deltaTime/tackleTime;
		return false;
	}
	else if (atkTimer_ < chargeTime + tackleTime+ backTobeforePosTime)
	{
		deltaPos_ -= attackVec2_ * Common::TACKLE_DISTANCE * i_deltaTime / backTobeforePosTime;
		return false;
	}
	atkTimer_ = 0.0;
	return true;
}

void Enemy::UpdateHPBar()
{
	HPBar_ = { {position_.x - 14,position_.y - 21 }, 28* HP_/Common::EnemyMaxHP ,3 };
}
bool Enemy::UpdateDeath()
{
	if (HP_ <= 0)
	{
		if(!isDying_)
		{
			isDying_ = true;
			this->GetComponent<CollisionComponent, COLLISION>()->Edit().isActivate = false;
			SetAttackInActive();
			effect_.add<DeathEffect>(this);
		}
		if(effect_.isEmpty())
		{
			isDead_ = true;			
		}
		return true;
	}
	return false;
}


void Enemy::UpdateMove(double i_deltaTime)
{	
	isMoving_ = false;
	if(isMoveable_)
	{
		Vec2 diff = wayPos_ - designatedPos_;

		if (diff.lengthSq() > DBL_EPSILON)
		{
			isMoving_ = true;
			if (diff.x > 0 && diff.x > std::abs(diff.y))//右
			{
				movement_.direction = 2;
			}
			else if (diff.x < 0 && std::abs(diff.x) > std::abs(diff.y))//左
			{
				movement_.direction = 1;
			}
			else if (diff.y > 0 && diff.y > std::abs(diff.x))//下
			{
				movement_.direction = 0;
			}
			else//上
			{
				movement_.direction = 3;
			}
			double diffLength = diff.length();
			//目標地点を通り過ぎないように、移動量は差分をより大きい場合は差分だけ移動させる
			deltaPos_ += diff / diffLength * std::min( movement_.currentSpeed * i_deltaTime,diffLength);
		}
		else
		{
			UpdateWayPos();
		}		
	}
}



void Enemy::Update(double i_deltaTime)
{
	if(!isDying_)
	{
		deltaPos_ = { 0,0 };
		designatedPos_ = position_;
		if (pController_)
		{
			pController_->Update(i_deltaTime);
		}
		if(isShowPathOn_)
		{
			agent_.ShowPath();
		}
		findPathTimer_ += i_deltaTime;
		if (findPathTimer_ > 1)
		{
			UpdatePath();
			findPathTimer_ = 0;
		}
		UpdateAttack(i_deltaTime);
		UpdateMove(i_deltaTime);
		UpdateHitting(i_deltaTime);
		UpdateHPBar();	
		UpdateMovement();
	}
	UpdateDeath();
}

void Enemy::UpdateRender(uint64 i_millisecTime)
{
	if(!isDying_)
	{
		//Circle{ position_,16 }.draw();
		int32 animationIndex = 1;
		//ミリ秒を割り算し余りでアニメーションの順番を指定
		const int32 n = (i_millisecTime / 100 % 4);
		animationIndex = Common::patterns[n];
		shadowTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
		if(isHitting_)
		{
			ColorF cm{ 3.0,0,0,1.0 };
			ScopedColorMul2D colorMul{ cm };
			enemyTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
		}else
		{
			enemyTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
		}
		HPBar_.draw(Palette::Red);
	}
}

void Enemy::UpdateWayPos()
{
	if (pathIndex_ < (agent_.path_).size())
	{
		wayPos_ = Common::NavMeshPosToPixelPos(agent_.path_[pathIndex_]);
		pathIndex_++;
	}
	else
	{
		UpdatePath();
	}
}

void Enemy::UpdatePath()
{
	if(agent_.GetNavMesh().isOnNavMesh(goalPos_))
	{
		agent_.path_.clear();
		const Vec2 start = Common::PixelPosToNavMeshPos(position_);
		agent_.FindPath(start, Common::PixelPosToNavMeshPos(goalPos_));
		pathIndex_ = 1;
		if (pathIndex_ < (agent_.path_).size())
		{
			wayPos_ = Common::NavMeshPosToPixelPos(agent_.path_[pathIndex_]);
			pathIndex_++;
		}
	}
}



