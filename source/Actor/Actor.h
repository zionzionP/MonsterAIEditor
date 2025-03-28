#pragma once
#include <Siv3D.hpp>
#include <unordered_map>
#include "Component/Component.h"
#include "Component/ComponentFactoryManager.h"
#include "Actor/ActorManager.h"

class ActorManager;
//class ComponentFactoryManager;
class CollisionComponent;
struct BlockingHitPoint
{
	BlockingHitPoint(const Vec2& i_hitPoint, CollisionComponent* i_pHitCollision) :hitPoint(i_hitPoint), hitCollision(i_pHitCollision) {}
	~BlockingHitPoint() = default;

	Vec2 hitPoint{ 0,0 };
	CollisionComponent* hitCollision = nullptr;
};

class Actor {
public:
	Actor();
	virtual ~Actor();
	//アクターの位置を取得
	const Vec2& GetPosition() const { return position_; }
	//アクターの位置をセット
	void SetPosition(const Vec2& i_pos) { position_ = i_pos; }
	//Designated位置を取得（AI等で使わないように）
	const Vec2& GetDesignatedPos() const { return designatedPos_; }
	//Designated位置をセット（AI等で使わないように）
	void SetDesignatedPos(const Vec2& i_pos) { designatedPos_ = i_pos; }
	//ノックバックターゲット位置をセット
	void SetKnockBackVec2(const Vec2& i_pos) { knockBackVec2_ = i_pos; }
	//アクターの削除フラグを取得
	const bool GetDead() const { return isDead_; }
	//アクターの削除フラグをセット
	void SetDead(bool in) { isDead_ = in; }
	//アクターの移動可能フラグを取得
	const bool GetMoveable() const { return isMoveable_; }
	//アクターの移動可能フラグをセット
	void SetMoveable(bool in) { isMoveable_ = in; }
	//アクターが移動中フラグを取得
	const bool GetMoving() const { return isMoving_; }
	//アクターが攻撃されるフラグを取得
	const bool GetHitting() const { return isHitting_; }
	//アクターが攻撃されるフラグをセット
	void SetHitting(bool in){ isHitting_ = in; }
	//アクターが被攻撃できるフラグを取得
	const bool GetDamageable() const { return isDamageable_; }
	//アクターが被攻撃できるフラグをセット
	void SetDamageable(bool in) { isDamageable_ = in; }
	const bool GetAgainstWall()const { return isAgainstWall_; }
	void SetAgainstWall(bool in) { isAgainstWall_ = in; }
	const bool GetIsShowPathOn()const { return isShowPathOn_; }
	void SetIsShowPathOn(bool in) { isShowPathOn_ = in; }
	//アクターのHPを取得
	const int GetHP() const { return HP_; }	
	//アクターのHPをセット
	void SetHP(int i_HP) { HP_ = i_HP; }
	//アタックIDを取得
	const int GetAttackId() const { return attackId_; }
	//アタックIDをセット
	void SetAttackId(int i_attackId) { attackId_ = i_attackId; }
	//ダメージIDを取得
	const int GetLastAttackId() const { return lastAttackId_; }
	//ダメージIDをセット
	void SetLastAttackId(int i_damageId) { lastAttackId_ = i_damageId; }
	auto& GetDeltaPositions() { return DeltaPositions_; }
	//コンポーネントを取得
	template<class ComponentType, int EComponentType>
	ComponentType* GetComponent()
	{
		auto itr = components_.find(EComponentType);
		if (itr == components_.end()) {
			return nullptr;
		}
		return static_cast<ComponentType*>(ComponentFactoryManager::Get()->GetComponent(EComponentType,(*itr).second));
	}

	template<class ComponentType, int EComponentType>
	const ComponentType* GetComponent() const
	{
		auto itr = components_.find(EComponentType);
		if (itr == components_.end()) {
			return nullptr;
		}
		return static_cast<ComponentType*>(ComponentFactoryManager::Get()->GetComponent(EComponentType , (*itr).second));
	}

	template<class ComponentType, int EComponentType>
	ComponentType* AddComponent()
	{
		ComponentFactoryManager* pManager = ComponentFactoryManager::Get();
		int componentIndex = pManager->CreateComponent(EComponentType, this);
		components_.insert({ EComponentType,componentIndex });
		return static_cast<ComponentType*>(pManager->GetComponent(EComponentType, componentIndex));
	}
	void UpdateComponentReference(int i_type, int i_index);
	//攻撃される時の処理
	virtual void OnHitted(int i_collisionTag, const Actor* i_pOther, int i_hpEffect, int i_attackId) {}
	virtual void Update(double i_deltaTime) {}
	virtual void UpdateRender(uint64 i_millisecTime) {}
	virtual void UpdateEffect() {}
	void AddDeltaPos();

	const BlockingHitPoint Sweep(const Vec2& i_start, const Vec2& i_remainVelocity);
	void  UpdateMovement();

	
protected:
	//削除フラグ
	bool isDead_ = false;
	//移動可能フラグ
	bool isMoveable_ = true;
	//移動中フラグ
	bool isMoving_ = false;
	//攻撃される途中フラグ
	bool isHitting_ = false;
	bool isDamageable_ = true;
	bool isAgainstWall_ = false;
	bool isShowPathOn_ = false;
	double damageTimer_ = 0.0;
	//アクターの位置
	Vec2 position_ = { 0, 0 };
	//Update中使う位置（AI等で使わないように）
	Vec2 designatedPos_ = { 0,0 };
	//あたり判定発生時ノックバックの方向ベクトル
	Vec2 knockBackVec2_ = { 0, 0 };
	//このフレームのUpdate中の移動量
	Vec2 deltaPos_ = { 0,0 };
	std::vector<Vec2> DeltaPositions_;
	//ノックバック用タイマー
	double KBTimer = 0.0;
	//HP
	int HP_ = 0;
	//
	int attackId_ = 0;
	int lastAttackId_ = 0;
	// 保有するコンポーネント(first:コンポーネントタイプ, second:そのタイプのFactoryで何番目に保存されてる)
	std::unordered_map<int,int> components_; 
	const Texture shadowTexture{ U"Assets/pipo-shadow001.png" };
};
