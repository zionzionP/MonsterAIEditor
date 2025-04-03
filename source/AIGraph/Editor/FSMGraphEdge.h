#pragma once
#include "AIGraph/Editor/FSMGraphObject.h"
#include "AIGraph/Editor/Node/FSMGraphNode.h"
#include "AIGraph/Editor/FSMGraphEdgeProperty.h"
#include "AIGraph/StateMachine/AIFactory.h"

class FSMGraphEdgeProperty;
class FSMGraphDecision;

/// @brief FSMグラフのエッジクラス
class FSMGraphEdge : public FSMGraphObject
{
public:
	/// @brief コンストラクタ
	/// @param pFrom エッジの始点ノード
	/// @param pTo エッジの終点ノード
	FSMGraphEdge(FSMGraphNode* pFrom, FSMGraphNode* pTo);
	/// @brief デストラクタ
	~FSMGraphEdge() override = default;

	/// @brief エッジのインデックスを取得する
	/// @return エッジのインデックス
	int32 GetIndex() const { return edgeIndex_; }
	/// @brief エッジのインデックスを設定する
	/// @param index エッジのインデックス
	void SetEdgeIndex(int32 index) { edgeIndex_ = index; }
	/// @brief 遷移優先度を取得する
	/// @return 遷移優先度
	int32 GetTransitionPriority() const { return transitionPriority_; }
	/// @brief 遷移優先度を設定する
	/// @param in 遷移優先度
	void SetTransitionPriority(int32 in) { transitionPriority_ = in; }
	/// @brief エッジの始点ノードを取得する
	/// @return エッジの始点ノード
	FSMGraphNode* GetFromNode() const { return pFromNode; }
	/// @brief エッジの終点ノードを取得する
	/// @return エッジの終点ノード
	FSMGraphNode* GetToNode() const { return pToNode; }
	/// @brief エッジのプロパティを取得する
	/// @return エッジのプロパティ
	FSMGraphEdgeProperty& GetProperty() { return property_; }
	/// @brief アタッチされた遷移条件を取得する
	/// @return アタッチされた遷移条件
	auto* GetAttachedDecision() const { return pAttachedDecision_; }
	/// @brief 遷移条件をアタッチする
	/// @param pDecision アタッチする遷移条件
	void SetAttachedDecision(FSMGraphDecision* pDecision) { pAttachedDecision_ = pDecision; }

	/// @brief 更新処理
	void Update() override;
	/// @brief 描画処理
	void Draw() override;
	/// @brief デバッグ描画処理
	/// @param pActor アクター
	void DebugDraw(Actor* pActor);
	/// @brief 遷移条件がアタッチの場合、小さい点を描画
	void DrawIfHasAttachedDecision();
	/// @brief クリックされたかどうかを判定する
	/// @return クリックされた場合はtrue、そうでない場合はfalse	
	bool IsClicked() const;
	/// @brief シリアライズ処理
	/// @return シリアライズされたJSONオブジェクト
	nlohmann::json Serialize() const;

private:
	int32 edgeIndex_ = 0; ///< エッジのインデックス
	int32 transitionPriority_ = 0; ///< 遷移優先度
	Line edgeLine_{ {0,0},{0,0} }; ///< エッジのライン
	double lineThickness = 4.0; ///< ラインの太さ
	FSMGraphNode* pFromNode = nullptr; ///< エッジの始点ノード
	FSMGraphNode* pToNode = nullptr; ///< エッジの終点ノード

	FSMGraphEdgeProperty property_; ///< エッジのプロパティ
	FSMGraphDecision* pAttachedDecision_ = nullptr; ///< アタッチされた遷移条件
};
