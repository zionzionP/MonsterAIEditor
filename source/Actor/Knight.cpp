#include "Actor/Knight.h"
#include "Core/Common.h"
#include "Effect/HitEffect.h"

void Knight::UpdateMove(double i_deltaTime)
{	
	isMoving_ = false;
	Vec2 moveVector = { 0,0 };
	if (isMoveable_)
	{		
		if (KeyS.pressed())   // S キー
		{
			isMoving_ = true;                    //移動フラグ更新
			movement_.direction = ECharacterDirection::CDOWN;             //キャラクター向き更新
			movement_.effectDirection = EEffectDirection::EDOWN;       //エフェクト方向更新
			moveVector += Common::moveVec2[movement_.direction]; //デルタ位置に加算
		}
		if (KeyW.pressed())  // W キー
		{
			isMoving_ = true;
			movement_.direction = ECharacterDirection::CUP;
			movement_.effectDirection = EEffectDirection::EUP;
			moveVector += Common::moveVec2[movement_.direction];
		}
		if (KeyA.pressed())  // A キー
		{
			isMoving_ = true;
			movement_.direction = ECharacterDirection::CLEFT;
			movement_.effectDirection = EEffectDirection::ELEFT;
			moveVector += Common::moveVec2[movement_.direction];
		}
		if (KeyD.pressed())  // D キー
		{
			isMoving_ = true;
			movement_.direction = ECharacterDirection::CRIGHT;
			movement_.effectDirection = EEffectDirection::ERIGHT;
			moveVector += Common::moveVec2[movement_.direction];
		}
		if (moveVector.lengthSq() > 0.0)
		{
			moveVector.normalize();
		}
		deltaPos_ += moveVector * movement_.currentSpeed * i_deltaTime;; //現在の位置を更新
	}	
}

void Knight::UpdateAttack(double i_deltaTime)
{
	//Attackコリジョンコンポーネント可視化(Debug用)
	/*if (pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Get().isActivate)
	{
		pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Get().hitbox.draw(Palette::Red);
	}*/
	if (ATKTimer >= 0.2)
	{
		pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Edit().isActivate = false;
	}
	//前回の攻撃エフェクトが再生完了前に攻撃不可能
	if (!isAttacking)
	{
		
		isMoveable_ = true;
		if (KeyJ.down()) // J キー
		{
			isMoveable_ = false;
			isAttacking = true;
			attackId_ += 1;
			pAttackActor_->SetAttackId(attackId_);
			ATKTimer = 0.0;
			//キャラクターに位置にエフェクトの位置を加算
			Vec2 pos = position_ + Common::attackVec2[movement_.direction];			
			pAttackActor_->SetDesignatedPos(pos);
			effect_.add<AttackEffect>(pAttackActor_, movement_.effectDirection);
			RectF& attackHitbox = pAttackActor_->GetComponent<CollisionComponent, COLLISION>()->Edit().hitbox;
			if(movement_.effectDirection == 1 || movement_.effectDirection == 3)
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
		Vec2 pos = position_ + Common::attackVec2[movement_.direction];
		pAttackActor_->SetDesignatedPos(pos);
		isMoveable_ = false;
		ATKTimer += i_deltaTime;
		if(ATKTimer > 0.3)
		{
			isAttacking = false;
		}
	}	
}

void Knight::OnHitted(int i_collisionTag, const Actor* i_pOther, int i_hpEffect, int i_attackId)
{
	if (i_collisionTag & Common::CollisionTag::COLLISION_HEALITEM)
	{
		HP_ = std::min(HP_ + i_hpEffect, Common::PlayerMaxHP);
		effect_.add<HealEffect>(designatedPos_,this);
	}
	else if (i_collisionTag & Common::CollisionTag::COLLISION_ENEMY)
	{
		if (isDamageable_)
		{
			HP_ = std::max(0, HP_ + i_hpEffect);
			isDamageable_ = false;
			Vec2 diff = designatedPos_ - i_pOther->GetDesignatedPos();//位置差分をとる
			if (diff.lengthSq() < FLT_EPSILON)//位置が重なった場合、位置差分を右に1マスにする
			{
				diff = { 32,32 };
			}
			knockBackVec2_ = diff / diff.length();//ノックバックの方向設定
			isHitting_ = true;
		}
	}
	UpdateHPBar();
}

//Collisionであたりを判定し、判定発生後の処理
void Knight::UpdateHitting(double i_deltaTime)
{
	if (isHitting_)
	{
		isMoveable_ = false;
		if (KnockBackDone(i_deltaTime))
		{
			isHitting_ = false;
			isMoveable_ = true;
		}
	}
}
//ノックバック処理
bool Knight::KnockBackDone(double i_deltaTime)
{
	KBTimer += i_deltaTime;
	if (KBTimer < 0.1)
	{
		//ノックバック時間(0.1)秒内ノックバック距離(100)を移動 
		deltaPos_ += knockBackVec2_  * Common::KNOCKBACK_DISTANCE  * i_deltaTime/0.1;
		return false;
	}
	KBTimer = 0.0;
	return true;
}

//無敵時間
void Knight::UpdateDamageable(double i_deltaTime)
{
	if(!isDamageable_)
	{
		damageTimer_ += i_deltaTime;
		if (damageTimer_ >= Common::undamageableTimeKinght)
		{
			isDamageable_ = true;
			damageTimer_ = 0.0;
		}
	}
}

void Knight::UpdateHPBar()
{
	HPBar_ = { {50,20 }, 200 * HP_ / Common::PlayerMaxHP ,10 };
}

void Knight::RenderHPBar()
{
	RectF{ {50,20 }, 200,10 }.draw();
	RoundRect{ {46,16 }, 208,18,4 }.draw(Palette::White);
	font(U"HP").draw(18, Vec2{ 20, 14 }, Palette::White);
	HPBar_.draw(Palette::Red);
}

void Knight::Update(double i_deltaTime)
{
	deltaPos_ = { 0,0 };
	designatedPos_ = position_;
	UpdateDamageable(i_deltaTime);
	UpdateHitting(i_deltaTime);
	UpdateMove(i_deltaTime);
	UpdateAttack(i_deltaTime);
	UpdateHPBar();
	UpdateMovement();
	
}

//キャラクターの描画
void Knight::UpdateRender(uint64 i_millisecTime)
{	
	//移動しない場合、立つアニメーションを再生
	int32 animationIndex = 1;
	//移動中、順番通りに移動のアニメーションを表示
	if (isMoving_)
	{
		//ミリ秒を割り算し余りでアニメーションの順番を指定
		const int32 n = (i_millisecTime / 50 % 4);
		animationIndex = Common::patterns[n];
	}
	
	shadowTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);	
	if (!isDamageable_ && i_millisecTime / 50 % 2 == 0)
	{
		ColorF cm{ 1.5,0,0,1.0 };
		ScopedColorMul2D colorMul{ cm };
		kinghtTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
	}else
	{
		kinghtTexture((32 * animationIndex), (32 * movement_.direction), 32, 32).drawAt(position_);
	}	
}


