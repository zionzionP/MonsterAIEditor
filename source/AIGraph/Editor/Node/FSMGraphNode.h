#pragma once
#include "AIGraph/Editor/FSMGraphObject.h"
#include "AIGraph/StateMachine/StateController.h"
#include "Core/json.hpp"

class FSMGraphEdge;

class FSMGraphNode : public FSMGraphObject
{
public:
	FSMGraphNode(){	};
	~FSMGraphNode() override = default;

public:
	/// @brief ノードの状態を更新する。
	void Update() override;

	/// @brief ノードを描画する。
	void Draw() override;

	/// @brief ノードがクリックされたかどうかを判定する。
	/// @return クリックされた場合は true、それ以外は false。
	bool IsClicked() const { return displayNode_.leftClicked(); }

	/// @brief ノードがクリックから解放されたかどうかを判定する。
	/// @return クリックが解放された場合は true、それ以外は false。
	bool IsReleased() const { return displayNode_.leftReleased(); }

public:
	/// @brief 表示ノードを取得
	const RectF& GetDisplayNode() const { return displayNode_; }

	/// @brief ノードの現在位置を取得する。
	/// @return ノードの座標。
	const Vec2& GetPosition() const { return position_; }

	/// @brief ノードの位置を設定する。
	/// @param i_pos 設定する座標。
	void SetPosition(const Vec2& i_pos) { position_ = i_pos; }

	/// @brief ノードの位置を更新し、表示ノードの中心を設定する。
	void RefreshPosition() { displayNode_.setCenter(position_); }

	/// @brief ノードのインデックスを取得する。
	/// @return ノードのインデックス。
	int32 GetNodeIndex() const { return nodeIndex_; }

	/// @brief ノードのインデックスを設定する。
	/// @param index 設定するインデックス。
	void SetNodeIndex(int32 index) { nodeIndex_ = index; }

	/// @brief ノードがドラッグ中かどうかを取得する。
	/// @return ドラッグ中の場合は true、それ以外は false。
	bool GetIsDragging() const { return isDragging_; }

	/// @brief ノードのドラッグ状態を設定する。
	/// @param in 設定する状態（true ならドラッグ中、false なら非ドラッグ）。
	void SetIsDragging(bool in) { isDragging_ = in; }

public:
	/// @brief ノードのアクションを StateController に登録する。
	/// @param io_controller アクションを登録する
	virtual void RegisterAction(StateController* io_controller) {}

	/// @brief ノードを JSON 形式にシリアライズする。
	/// @return シリアライズされた JSON データ。
	virtual nlohmann::json Serialize() const { return nlohmann::json(); }

	/// @brief JSON データからノードをデシリアライズする。
	/// @param i_json デシリアライズする JSON データ。
	virtual void Deserialize(const nlohmann::json& i_json) {}

	/// @brief プロパティ情報を更新する。
	virtual void UpdateProperty() {}

	/// @brief プロパティ情報を描画する。
	virtual void DrawProperty();

	/// @brief デバッグ用にアクター情報を描画する。
	/// @param pActor 描画対象のアクター。
	virtual void DebugDraw(Actor* pActor) {}

	/// @brief ノードの枠を描画する。
	void DrawFrame() const { displayNode_.drawFrame(0, 5, Palette::Yellow); }

private:
	/// @brief ノード内に表示する名前を描画する。
	void drawDisplayNameInNode();
protected:
	void splitDescription();

protected:
	String typeName_ = U""; ///< ノードの種類を表す。
	String displayName_ = U""; ///< ノードの表示名。
	String description_ = U""; ///< ノードの説明文
	std::vector<String> descriptionForDisplay_; ///< 一行ずつ表示するための説明文
	Color color_ = Palette::White; ///< ノードの色。
	RectF displayNode_{ Arg::center(0,0), 100, 60 }; ///< ノードの表示領域。
	const Font font{ FontMethod::MSDF, 30 }; ///< フォント設定。

	Vec2 position_ = { 0,0 }; ///< ノードの位置。
	int32 nodeIndex_ = -1; ///< ノードのインデックス。
	bool isDragging_ = false; ///< ドラッグ中かどうか。
};
