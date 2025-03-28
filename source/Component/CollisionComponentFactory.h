#pragma once
#include <vector>
#include <variant>
#include "Component/CollisionComponent.h"
#include "Component/ComponentFactoryInterface.h"

class CollisionComponent;
class CollisionComponentFactory :public  ComponentFactoryInterface
{
public:
	typedef std::vector<CollisionComponent> ComponentList;

	CollisionComponentFactory() :
		components_()
	{
		components_.reserve(100);
	}

	~CollisionComponentFactory() = default;

	const ComponentList& GetComponents() const { return components_; }
	ComponentList& GetComponents() { return components_; }
	//クリエイト関数
	int CreateComponent(Actor* io_pActor) override;

	Component* GetComponent(int i_index) override;

	bool RemoveComponent(int i_index) override;

	void Update();


protected:
	std::vector<CollisionComponent> components_;
};
