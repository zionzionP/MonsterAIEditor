#include "Component/CollisionComponentFactory.h"
//CollisionComponentを生成し、一般的なComponentのポインタを返す
int CollisionComponentFactory::CreateComponent(Actor* io_pActor)
{
	int index = static_cast<int>(components_.size());
	components_.emplace_back(io_pActor, this);
	return index;
}

Component* CollisionComponentFactory::GetComponent(int i_index)
{
	if (i_index < static_cast<int>(components_.size()))
	{
		return &components_[i_index];
	}
	return nullptr;
}
bool CollisionComponentFactory::RemoveComponent(int i_index)
{
	int size = static_cast<int>(components_.size());
	if (i_index < size)
	{
		if(i_index + 1 != size)
		{
			components_[i_index] = std::move(components_[size - 1]);
			components_[i_index].EditOwner().UpdateComponentReference(COLLISION, i_index);
		}
		components_.pop_back();
		return true;
	}
	return false;
}


//円と長方形の当たり判定（辺がくっつくだけの場合がfalse）
bool IntersectsCircleRect(const Circle& i_circle, const RectF& i_rect)
{
	Vec2 center = i_circle.center;
	Vec2 closestPoint = Common::ClosestPointAABB(center, i_rect);
	if (closestPoint.distanceFromSq(center) < pow(i_circle.r, 2))
	{
		return true;
	}
	else
	{
		return false;
	}
}


//Collision更新処理、Factoryで行う　Releaseモード　1000体の敵　Update一回　0.00035
void CollisionComponentFactory::Update()
{
	//Update前に、コリジョンの位置を更新
	for (auto& collision : components_)
	{
		if (collision.Get().collisionFlags != Common::CollisionTag::COLLISION_WALL)
		{
			collision.Edit().hitbox.setCenter(collision.GetOwner().GetDesignatedPos());
			collision.Edit().hitboxCircle.setCenter(collision.GetOwner().GetDesignatedPos());
		}
	}
	
	for (auto collision1 = components_.begin(); collision1 != components_.end(); collision1++)
	{
		for (auto collision2 = collision1 + 1; collision2 != components_.end(); collision2++)
		{
			//壁以外の当たり判定
			if (collision1->Get().isActivate && collision2->Get().isActivate &&
				collision1->Get().collisionFlags != Common::CollisionTag::COLLISION_WALL &&
				collision2->Get().collisionFlags != Common::CollisionTag::COLLISION_WALL)
			{
				if (collision1->Get().collisionTo & collision2->Get().collisionFlags
					|| collision1->Get().collisionFlags & collision2->Get().collisionTo)
				{
					const auto& hitbox1 = collision1->Get().hitbox;
					const auto& hitboxCircle1 = collision1->Get().hitboxCircle;
					const auto& hitbox2 = collision2->Get().hitbox;
					const auto& hitboxCircle2 = collision2->Get().hitboxCircle;
					bool isIntersected = false;
					if (collision1->Get().hitboxType == HitboxType::RECT)
					{
						if (collision2->Get().hitboxType == HitboxType::RECT)
						{
							//両方長方形の場合
							isIntersected = hitbox1.intersects(hitbox2);
						}
						else if (collision2->Get().hitboxType == HitboxType::CIRCLE)
						{
							//1が長方形、2が円の場合
							isIntersected = hitbox1.intersects(hitboxCircle2);
						}
					}
					else if (collision1->Get().hitboxType == HitboxType::CIRCLE)
					{
						if (collision2->Get().hitboxType == HitboxType::RECT)
						{
							//1が円、2が長方形の場合
							isIntersected = hitboxCircle1.intersects(hitbox2);
						}
						else if (collision2->Get().hitboxType == HitboxType::CIRCLE)
						{
							//両方円の場合
							isIntersected = hitboxCircle1.intersects(hitboxCircle2);
						}
					}

					if (isIntersected)
					{
						if(collision1->Get().collisionTo & collision2->Get().collisionFlags)
						{
							collision1->EditOwner().OnHitted(collision2->Get().collisionFlags, &collision2->GetOwner(), collision2->Get().hpEffect, collision2->GetOwner().GetAttackId());

						}
						if(collision2->Get().collisionTo & collision1->Get().collisionFlags)
						{
							collision2->EditOwner().OnHitted(collision1->Get().collisionFlags ,&collision1->GetOwner(), collision1->Get().hpEffect, collision1->GetOwner().GetAttackId());
						}						
					}										
				}
				else
				{
					continue;
				}
			}

			//壁との当たり判定
			if (collision1->Get().isActivate && collision2->Get().isActivate)
			{
				auto thisCollision = components_.begin();
				auto wallCollision = components_.begin();
				if (collision2->Get().collisionFlags == Common::CollisionTag::COLLISION_WALL)
				{
					thisCollision = collision1;
					wallCollision = collision2;
				}
				else if (collision1->Get().collisionFlags == Common::CollisionTag::COLLISION_WALL)
				{
					thisCollision = collision2;
					wallCollision = collision1;
				}
				else
				{
					continue;
				}
				if (thisCollision->Get().collisionTo == Common::CollisionTag::NOTHING)
				{
					continue;
				}
				if (thisCollision->Get().hitboxType == HitboxType::RECT)
				{
					//壁（長方形）と長方形が接触の場合
					if (thisCollision->Get().hitbox.intersects(wallCollision->Get().hitbox))
					{
						Vec2 diff = thisCollision->GetOwner().GetDesignatedPos() - wallCollision->GetOwner().GetDesignatedPos();//今のフレームでUpdate後のポジションの差分
						Vec2 diffPos = thisCollision->GetOwner().GetPosition() - wallCollision->GetOwner().GetPosition();//前フレームのバグのないPositionの差分
						double slope = diffPos.y / diffPos.x;//差分の勾配
						double wallSlope = (wallCollision->Get().hitbox.h + thisCollision->Get().hitbox.h) / (wallCollision->Get().hitbox.w + thisCollision->Get().hitbox.w);//壁+当たるコリジョンのサイズで引いた対角線の勾配

						if (diffPos.x > 0 && slope <= wallSlope && slope >= -wallSlope)
						{
							//右に接触
							thisCollision->EditOwner().SetDesignatedPos(thisCollision->GetOwner().GetDesignatedPos()
								+ Vec2{ (wallCollision->Get().hitbox.w / 2 - diff.x + thisCollision->Get().hitbox.w / 2), 0 });
						}
						else if (diffPos.x < 0 && slope <= wallSlope && slope >= -wallSlope)
						{
							//左に接触
							thisCollision->EditOwner().SetDesignatedPos(thisCollision->GetOwner().GetDesignatedPos()
								- Vec2{ (wallCollision->Get().hitbox.w / 2 + diff.x + thisCollision->Get().hitbox.w / 2), 0 });
						}
						else if (diffPos.y > 0 && (slope > wallSlope || slope < -wallSlope))
						{
							//下に接触
							thisCollision->EditOwner().SetDesignatedPos(thisCollision->GetOwner().GetDesignatedPos()
								+ Vec2{ 0, (wallCollision->Get().hitbox.h / 2 - diff.y + thisCollision->Get().hitbox.h / 2) });
						}
						else if (diffPos.y < 0 && (slope > wallSlope || slope < -wallSlope))
						{
							//上に接触
							thisCollision->EditOwner().SetDesignatedPos(thisCollision->GetOwner().GetDesignatedPos()
								- Vec2{ 0, (wallCollision->Get().hitbox.h / 2 + diff.y + thisCollision->Get().hitbox.h / 2) });
						}
					}
				}
				else if (thisCollision->Get().hitboxType == HitboxType::CIRCLE)
				{
					//壁（長方形）と円が接触の場合					
					if(IntersectsCircleRect(thisCollision->Get().hitboxCircle, wallCollision->Get().hitbox))
					{						
						const auto& thisCircle = thisCollision->Get().hitboxCircle;
						const auto& wall = wallCollision->Get().hitbox;
						Vec2 diff = thisCollision->GetOwner().GetDesignatedPos() - wallCollision->GetOwner().GetDesignatedPos();//今のフレームでUpdate後のポジションの差分
						Vec2 diffPos = thisCollision->GetOwner().GetPosition() - wallCollision->GetOwner().GetPosition();//前フレームのバグのないPositionの差分
						double slope = diffPos.y / diffPos.x;//差分の勾配
						double wallSlope = wallCollision->Get().hitbox.h/ wallCollision->Get().hitbox.w;
						

						if (Vec2 diffTR = thisCircle.center - wall.tr();diffTR.x >= 0 && diffTR.y <= 0)
						{
							//右上に接触
							thisCollision->EditOwner().GetDeltaPositions().emplace_back(diffTR / diffTR.length() * (thisCircle.r - diffTR.length()));
						}
						else if (Vec2 diffBR = thisCircle.center - wall.br(); diffBR.x >= 0 && diffBR.y >= 0)
						{
							//右下に接触
							thisCollision->EditOwner().GetDeltaPositions().emplace_back(diffBR / diffBR.length() * (thisCircle.r - diffBR.length()));
						}
						else if (Vec2 diffBL = thisCircle.center - wall.bl(); diffBL.x <= 0 && diffBL.y >= 0)
						{
							//左下に接触
							thisCollision->EditOwner().GetDeltaPositions().emplace_back(diffBL / diffBL.length() * (thisCircle.r - diffBL.length()));
						}
						else if (Vec2 diffTL = thisCircle.center - wall.tl(); diffTL.x <= 0 && diffTL.y <= 0)
						{
							//左上に接触
							thisCollision->EditOwner().GetDeltaPositions().emplace_back(diffTL / diffTL.length() * (thisCircle.r - diffTL.length()));
						}
						else if (diffPos.x > 0 && slope <= wallSlope && slope >= -wallSlope)
						{
							//右に接触							
							thisCollision->EditOwner().GetDeltaPositions().emplace_back(Vec2{ (wall.w / 2 - diff.x + thisCircle.r), 0 });
						}
						else if (diffPos.x < 0 && slope <= wallSlope && slope >= -wallSlope)
						{
							//左に接触							
							thisCollision->EditOwner().GetDeltaPositions().emplace_back(Vec2{ -(wall.w / 2 + diff.x + thisCircle.r), 0 });

						}
						else if (diffPos.y > 0 && (slope > wallSlope || slope < -wallSlope))
						{
							//下に接触
							thisCollision->EditOwner().GetDeltaPositions().emplace_back(Vec2{ 0, (wall.h / 2 - diff.y + thisCircle.r) });

						}
						else if (diffPos.y < 0 && (slope > wallSlope || slope < -wallSlope))
						{
							//上に接触
							thisCollision->EditOwner().GetDeltaPositions().emplace_back(Vec2{ 0, -(wall.h / 2 + diff.y + thisCircle.r) });

						}

					}
				}
			}	
		}
	}		
}



