#pragma once
#include "Actor/Actor.h"
class Actor;

class Component
{
public:
	const Actor& GetOwner() const { return *pOwnerActor_; }
	Actor& EditOwner() { return *pOwnerActor_; }

protected:
	virtual ~Component() {}
	Component(Actor* io_actor):
		pOwnerActor_(io_actor) {}

private:
	friend Actor;
	Actor* pOwnerActor_;
};
