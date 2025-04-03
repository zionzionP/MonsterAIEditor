#pragma once
#include "AIGraph/Editor/Node/FSMGraphNode.h"
#include "FSMGraphEdge.h"
#include "AIGraph/Editor/Decision/FSMGraphDecision.h"
#include "Core/Common.h"
#include "AIGraph/Editor/UI/FSMGraphAddDecisionPanel.h"
#include "AIGraph/Editor/UI/FSMGraphAddNodePanel.h"
#include "AIGraph/Editor/FSMGraphDebug.h"
#include "AIGraph/Editor/UI/FSMGraphSelectEnemyPanel.h"
#include "Level/Setup.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

/// @brief FSMグラフクラス
class FSMGraph
{
	typedef std::vector<std::shared_ptr<FSMGraphNode>> NodeList;
	typedef std::vector<std::shared_ptr<FSMGraphEdge>> EdgeList;
	typedef std::vector<std::shared_ptr<FSMGraphDecision>> DecisionList;
public:
	/// @brief コンストラクタ
	FSMGraph() :
		addNodePanel_(this),
		addDecisionPanel_(this),
		selectEnemyPanel_(this),
		debug_(this)
	{
	};
	/// @brief デストラクタ
	~FSMGraph() = default;

	/// @brief 初期化処理
	void Initialize();
	/// @brief 更新処理
	void Update();
	/// @brief 描画処理
	void Draw();

	/// @brief ノードを追加する
	/// @param spNode 追加するノード
	void AddNode(const std::shared_ptr<FSMGraphNode>& spNode);
	/// @brief 遷移条件を追加する
	/// @param spDecision 追加する遷移条件
	void AddDecision(const std::shared_ptr<FSMGraphDecision>& spDecision);
	/// @brief エッジを追加する
	/// @param spEdge 追加するエッジ
	void AddEdge(const std::shared_ptr<FSMGraphEdge>& spEdge);

	/// @brief ノードを削除する
	/// @param pNode 削除するノード
	void RemoveNode(FSMGraphNode* pNode);
	/// @brief エッジを削除する
	/// @param pEdge 削除するエッジ
	void RemoveEdge(FSMGraphEdge* pEdge);
	/// @brief 遷移条件を削除する
	/// @param pDecision 削除する遷移条件
	void RemoveDecision(FSMGraphDecision* pDecision);

	/// @brief ノードリストを取得する
	/// @return ノードリスト
	const auto& GetNodes() const { return nodes_; }
	/// @brief エッジリストを取得する
	/// @return エッジリスト
	const auto& GetEdges() const { return edges_; }
	/// @brief 遷移条件リストを取得する
	/// @return 遷移条件リスト
	const auto& GetDecisions() const { return decisions_; }

	/// @brief ドラッグ中の遷移条件を設定する
	/// @param pDecision ドラッグ中の遷移条件
	void SetDraggingDecision(FSMGraphDecision* pDecision);
	/// @brief セットアップを取得する
	/// @return セットアップ
	Setup& GetSetup() { return setup; }
	/// @brief 現在の敵キャラクターを取得する
	/// @return 現在の敵キャラクター
	Enemy* GetCurrentEnemy() const { return pCurrentEnemy_; }
	/// @brief 現在の敵キャラクターを設定する
	/// @param pEnemy 現在の敵キャラクター
	void SetCurrentEnemy(Enemy* pEnemy) { pCurrentEnemy_ = pEnemy; }
	/// @brief グラフをクリアする
	void ClearGraph()
	{
		nodes_.clear();
		edges_.clear();
		decisions_.clear();
	}

private:
	/// @brief イベントを処理する
	void handleEvents();
	/// @brief ノード間にエッジを追加する
	void addEdgeBetweenNodes();
	/// @brief カーソルクリック時の処理
	void onCursorClick();
	/// @brief ノードをドラッグする
	void dragNode();
	/// @brief 遷移条件をドラッグする
	void dragDecision();
	/// @brief ノードを選択する
	/// @return 選択された場合はtrue、そうでない場合はfalse
	bool selectNode();
	/// @brief エッジを選択する
	/// @return 選択された場合はtrue、そうでない場合はfalse
	bool selectEdge();
	/// @brief 遷移条件を選択する
	/// @return 選択された場合はtrue、そうでない場合はfalse
	bool selectDecision();
	/// @brief 選択されたノードを削除する
	void removeSelectedNode();
	/// @brief 選択されたエッジを削除する
	void removeSelectedEdge();
	/// @brief 選択された遷移条件を削除する
	void removeSelectedDecision();
	/// @brief エッジに遷移条件をアタッチする
	void attachDecisionToEdge();
	/// @brief グラフをコンパイルしてデバッグを開始する
	void compileGraphAndStartDebug();
	/// @brief デバッグ停止
	void stopDebug();
	/// @brief FSMグラフを保存する
	void saveFSMGraph();
	/// @brief FSMグラフをロードする
	void loadFSMGraph();
	/// @brief FSMグラフファイルを選択してロードする
	void selectAndLoadFSMGraphFile();

	/// @brief デバッグエリアを更新する
	void updateDebugArea();
	/// @brief デバッグエリアを描画する
	void drawDebugArea();
	/// @brief UIを描画する
	void drawUI();
	/// @brief エッジ作成中の描画
	void drawCreatingEdge();

private:
	bool isAddingEdge_ = false;
	NodeList nodes_;
	EdgeList edges_;
	DecisionList decisions_;

	FSMGraphNode* pSelectedNode_ = nullptr;
	FSMGraphEdge* pSelectedEdge_ = nullptr;
	FSMGraphDecision* pSelectedDecision_ = nullptr;
	FSMGraphDecision* pDraggingDecision_ = nullptr;
	bool canAttachDecision_ = false;

	FSMGraphNode* pFromNodeForCreate = nullptr;
	FSMGraphNode* pToNodeForCreate = nullptr;	

	Effect effect_;

	//UI
	RectF debugArea{ 5,100,575,550 };
	Rect graphWindow{ 585, 100, 750, 550 };
	Rect propertyWindow{ 1340, 100, 255, 550 };
	FSMGraphAddNodePanel addNodePanel_;
	FSMGraphAddDecisionPanel addDecisionPanel_;
	FSMGrapSelectEnemyPanel selectEnemyPanel_;
	const Font font{ FontMethod::MSDF, 48 };
	//saveload用
	Optional<FilePath> fsmgFilePath_;
	String fsmgFileName_ = U"";
	TextEditState filePathTe_;
	//DEBUGエリア用
	bool isDebugging_ = false;
	double deltaTime = 0;
	uint64 millisecTime = 0;
	ActorManager* pManager = ActorManager::Get();
	ComponentFactoryManager* pCManager = ComponentFactoryManager::Get();
	Setup setup;
	Enemy* pCurrentEnemy_ = nullptr;
	StateController* pCompiledController_ = nullptr;
	friend FSMGraphDebug;
	FSMGraphDebug debug_;
};
