#pragma once
#include "Core/Singleton.h"
#include "Component/ComponentFactoryInterface.h"
#include <unordered_map>
enum EComponentType {
	COLLISION = 0,
};

class ComponentFactoryManager : public Singleton<ComponentFactoryManager>
{
public:
	typedef std::unordered_map<int, std::unique_ptr<ComponentFactoryInterface>> FactoryMap;

	ComponentFactoryManager(){}
	~ComponentFactoryManager() = default;

	void AddFactory(int i_type, ComponentFactoryInterface* io_pInterface);
	int CreateComponent(int i_type, Actor* i_pActor);
	Component* GetComponent(int i_type, int i_index);
	bool RemoveComponent(int i_type, int i_index) const;

	void Update();

private:
	//(first: EComponentType, second: ComponentFactoryInterface*)
	FactoryMap factoryMap_;
};
