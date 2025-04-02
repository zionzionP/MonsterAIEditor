#include "Actor/SkeletonKnight.h"

void SkeletonKnight::Update(double i_deltaTime)
{
	if(!isDying_)
	{
		deltaPos_ = { 0,0 };
		designatedPos_ = position_;
		if (pController_)
		{
			pController_->Update(i_deltaTime);
		}
		if (isShowPathOn_)
		{
			agent_.ShowPath();
		}
		findPathTimer_ += i_deltaTime;
		if (findPathTimer_ > 0.5)
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

void SkeletonKnight::SetAttackInActive()
{
	pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Edit().isActivate = false;
}

void SkeletonKnight::UpdateAttack(double i_deltaTime)
{
	//Attackコリジョンコンポーネント可視化(Debug用)
	/*if(pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Get().isActivate)
	{
		pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Get().hitbox.draw(Palette::Red);
	}*/
	if (ATKTimer >= 0.2)
	{
		pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Edit().isActivate = false;
	}
	if (effect_.isEmpty())
	{		
		if (isAttacking_)
		{
			Vec2 diff = pTarget_->GetPosition() - position_;
			if (diff.x > 0 && diff.x > std::abs(diff.y))//右
			{
				movement_.direction = 2;
				movement_.effectDirection = 3;
			}
			else if (diff.x < 0 && std::abs(diff.x) > std::abs(diff.y))//左
			{
				movement_.direction = 1;
				movement_.effectDirection = 1;
			}
			else if (diff.y > 0 && diff.y > std::abs(diff.x))//下
			{
				movement_.direction = 0;
				movement_.effectDirection = 0;
			}
			else//上
			{
				movement_.direction = 3;
				movement_.effectDirection = 2;
			}
			attackId_ += 1;
			pAttackActor_->SetAttackId(attackId_);
			ATKTimer = 0.0;
			//キャラクターに位置にエフェクトの位置を加算
			Vec2 pos = position_ + Common::attackVec2[movement_.direction];			
			pAttackActor_->SetDesignatedPos(pos);
			addAttackEffect();
			RectF& attackHitbox = pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Edit().hitbox;
			if (movement_.effectDirection == 1 || movement_.effectDirection == 3)
			{
				attackHitbox.h = 50;
				attackHitbox.w = 36;
			}
			else
			{
				attackHitbox.h = 36;
				attackHitbox.w = 50;
			}
			pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Edit().isActivate = true;			
		}
	}
	else
	{
		//AttackActor位置更新
		isMoveable_ = false;
		isAttacking_ = false;
		Vec2 pos = position_ + Common::attackVec2[movement_.direction];
		pAttackActor_->SetDesignatedPos(pos);
		ATKTimer += i_deltaTime;
	}
}

void SkeletonKnight::UpdateHPBar()
{
	HPBar_ = { {position_.x - 14,position_.y - 21 }, 28 * HP_ / Common::SkeletonMaxHP ,3 };
}

void SkeletonKnight::PhaseTransform()
{
	if(!isDying_)
	{
		isPhaseTransformed = true;
		HP_ = Common::SkeletonMaxHP;
		effect_.add<RangeAttackEffectSkeleton>(position_);
		const double newSpeed = 220;
		movement_.currentSpeed = newSpeed;
	}
}
void SkeletonKnight::UpdateRender(uint64 i_millisecTime)
{
	if (!isDying_)
	{
		int32 animationIndex = 1;
		if (isMoving_)
		{
			//ミリ秒を割り算し余りでアニメーションの順番を指定
			const int32 n = (i_millisecTime / 100 % 4);
			animationIndex = Common::patterns[n];
		}
		if (isPhaseTransformed)
		{
			const int32 n = (i_millisecTime / 100 % 8);
			auraTexture((120 * n), 0, 120, 150).drawAt(position_);
		}

		shadowTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
		if (isHitting_)
		{
			ColorF cm{ 3.0,0,0,1.0 };
			ScopedColorMul2D colorMul{ cm };
			enemyTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
		}
		else if (isPhaseTransformed)
		{
			ColorF cm{ 0.8,0,0.2,1.0 };
			ScopedColorMul2D colorMul{ cm };
			enemyTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
		}
		else
		{
			enemyTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
		}

		HPBar_.draw(Palette::Red);
	}
}
