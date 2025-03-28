#include "Actor/ActorManager.h"

ActorManager::ActorManager():
	actors_()
{
	actors_.reserve(100);
}
void ActorManager::AddActor(Actor* i_pActor)
{
	if (i_pActor != nullptr)
	{
		actors_.push_back(i_pActor);
	}	
}

void ActorManager::RemoveActor(Actor* i_pActor)
{
	if (i_pActor != nullptr)
	{
		auto itr = std::find(actors_.begin(), actors_.end(), i_pActor);
		if (itr != actors_.end())
		{
			actors_.erase(itr);
		}
	}
}

void ActorManager::UpdateRender(uint64 i_millisecTime)
{
	if (!actors_.empty())
	{
		for (Actor* pActor : actors_)
		{
			if (pActor != nullptr)
			{
				pActor->UpdateRender(i_millisecTime);
			}
		}
	}
}

void ActorManager::Update(double i_deltaTime)
{
	if (!actors_.empty())
	{		
		for (Actor* pActor : actors_)
		{
			if (pActor != nullptr)
			{				
				pActor->Update(i_deltaTime);
			}
		}
		auto itr = actors_.begin();
		while (itr != actors_.end())
		{
			if ((*itr)->GetDead())
			{
				delete* itr;
				itr = actors_.erase(itr);
			}
			else
			{
				itr++;
			}
		}
	}
}

void ActorManager::UpdatePosition()
{
	if (!actors_.empty())
	{
		for (Actor* pActor : actors_)
		{
			if (pActor != nullptr)
			{
				pActor->AddDeltaPos();
				pActor->SetPosition(pActor->GetDesignatedPos());
				pActor->GetDeltaPositions().clear();
			}
		}
	}
}

void ActorManager::UpdateEffect()
{
	if (!actors_.empty())
	{
		for (Actor* pActor : actors_)
		{
			if (pActor != nullptr)
			{
				pActor->UpdateEffect();				
			}
		}
	}
}


void ActorManager::GeneWallList()
{
	wallList_ = actors_;
}

void ActorManager::DrawWallCollision()
{
	for (const Actor* pWall : wallList_)
	{
		auto pCollision = pWall->GetComponent<CollisionComponent, COLLISION>();
		pCollision->Get().hitbox.draw(Palette::Black);		
	}
}

void ActorManager::ClearActor()
{
	for (auto itr = actors_.begin(); itr != actors_.end(); itr++)
	{
		if(*itr != nullptr) 
		{
			delete* itr;
		}
	}
	actors_.clear();
	wallList_.clear();
}

void ActorManager::ShowPathOn()
{
	for (auto actor : actors_)
	{
		actor->SetIsShowPathOn(true);
	}
}
void ActorManager::ShowPathOff()
{
	for (auto actor : actors_)
	{
		actor->SetIsShowPathOn(false);
	}
}
