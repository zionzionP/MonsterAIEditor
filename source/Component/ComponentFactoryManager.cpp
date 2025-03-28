#include "Component/ComponentFactoryManager.h"

void ComponentFactoryManager::AddFactory(int i_type, ComponentFactoryInterface* i_pInterface)
{
	factoryMap_.emplace(i_type, std::unique_ptr<ComponentFactoryInterface>(i_pInterface));
}

//typeに対応するFactoryを選択し、クリエイト関数を呼び出す
int ComponentFactoryManager::CreateComponent(int i_type, Actor* i_pActor)
{
	 auto itr = factoryMap_.find(i_type);
	 return itr->second->CreateComponent(i_pActor);
}

Component* ComponentFactoryManager::GetComponent(int i_type, int i_index)
{
	auto itr = factoryMap_.find(i_type);
	return itr->second->GetComponent(i_index);
}

bool ComponentFactoryManager::RemoveComponent(int i_type, int i_index) const
{
	auto itr = factoryMap_.find(i_type);
	return itr->second->RemoveComponent(i_index);
}
//全Factoryに対してComponent更新処理
void ComponentFactoryManager::Update()
{
	for (auto& factory : factoryMap_)
	{
		(factory.second)->Update();
	}
}
