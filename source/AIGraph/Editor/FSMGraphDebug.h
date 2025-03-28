#pragma once

class FSMGraph;

/// @brief FSMグラフのデバッグクラス
class FSMGraphDebug
{
public:
	/// @brief コンストラクタ
	/// @param i_pGraph 所有するFSMグラフ
	FSMGraphDebug(FSMGraph* i_pGraph) : pOwnerGraph_(i_pGraph) {};
	/// @brief デストラクタ
	~FSMGraphDebug() = default;

	/// @brief 更新処理
	void Update();
	/// @brief デバッグ描画処理
	void DebugDraw();

	/// @brief ナビメッシュのデバッグ描画
	void DebugDrawNavmesh();
	/// @brief パスのデバッグ描画
	void DebugDrawPath();
	/// @brief 距離範囲のデバッグ描画
	void DebugDrawDistanceRange();
	/// @brief アクティブノードのデバッグ描画
	void DebugDrawActiveNode();

private:
	FSMGraph* pOwnerGraph_ = nullptr;
	bool isDebugDrawNavmesh_ = false;
	bool isDebugDrawPath_ = false;
	bool isDebugDrawDistanceRange_ = false;
	bool isDebugDrawActiveNode_ = false;

};
