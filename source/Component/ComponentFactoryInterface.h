#pragma once
class Actor;
class Component;

class ComponentFactoryInterface {
public:
	virtual ~ComponentFactoryInterface() {}
	virtual void Update() = 0;
	virtual int CreateComponent(Actor* io_pActor) = 0;
	virtual bool RemoveComponent(int i_index) = 0;
	virtual Component* GetComponent(int i_index) = 0;
};
