#include "AIGraph/Editor/FSMGraph.h"

#include "Effect/CursorEffect.h"
#include "AIGraph/Editor/FSMCompiler.h"
#include "AIGraph/Editor/Node/Action/FSMGraphAttackActionNode.h"
#include "AIGraph/Editor/Decision/FSMGraphDistanceDecision.h"
#include "AIGraph/Editor/FSMGraphSaveLoad.h"
#include "AIGraph/Editor/Node/Action/FSMGraphWaitActionNode.h"
#include <filesystem>

#include "Level/LevelManager.h"


void FSMGraph::Update()
{
	handleEvents();
	selectEnemyPanel_.Update();

	for (auto& spNode : nodes_)
	{
		spNode->Update();
	}
	for (auto& spEdge : edges_)
	{
		spEdge->Update();
	}
	for (auto& spDecision : decisions_)
	{
		spDecision->Update();
	}
	updateDebugArea();
	debug_.Update();
}

void FSMGraph::Draw()
{
	drawUI();
	debug_.DebugDraw();
	for (auto& spEdge : edges_)
	{
		spEdge->Draw();
	}
	for (auto& spNode : nodes_)
	{
		spNode->Draw();
	}
	for (auto& spDecision : decisions_)
	{
		if (spDecision.get()->GetIsAttached())
		{
			continue;
		}
		spDecision->Draw();
	}
	drawCreatingEdge();
	drawDebugArea();
	
	effect_.update();
}
void FSMGraph::drawUI()
{
	debugArea.draw(ColorF{ 1.0, 1.0, 1.0, 0.5 });
	graphWindow.draw(ColorF{ 1.0, 1.0, 1.0, 0.5 });
	propertyWindow.draw(ColorF{ 1.0, 1.0, 1.0, 0.5 });	
	if (pSelectedEdge_)
	{
		addDecisionPanel_.DrawAddDecisionButton();
	}
	else
	{
		addNodePanel_.DrawAddNodeButton();
	}
	selectEnemyPanel_.Draw();
}

void FSMGraph::drawCreatingEdge()
{
	if (pFromNodeForCreate)
	{
		Line{ pFromNodeForCreate->GetPosition(),Cursor::Pos() }.drawArrow(4, { 10,10 }, Palette::White);
	}
}

void FSMGraph::Initialize()
{
	setup.Remesh();
}

void FSMGraph::AddNode(const std::shared_ptr<FSMGraphNode>& spNode)
{
	nodes_.push_back(spNode);
	spNode->SetNodeIndex(nodes_.size() - 1);
}

void FSMGraph::AddEdge(const std::shared_ptr<FSMGraphEdge>& spEdge)
{
	edges_.push_back(spEdge);
	spEdge->SetEdgeIndex(edges_.size() - 1);
}

void FSMGraph::AddDecision(const std::shared_ptr<FSMGraphDecision>& spDecision)
{
	decisions_.push_back(spDecision);
	spDecision->SetDecisionIndex(decisions_.size() - 1);	
}

void FSMGraph::RemoveNode(FSMGraphNode* pNode)
{
	//ノードに繋がるエッジにつけてる遷移条件を削除
	std::ranges::for_each(edges_.begin(), edges_.end(), [this, pNode](const std::shared_ptr<FSMGraphEdge>& spEdge)
	{
		if (spEdge.get()->GetFromNode() == pNode || spEdge.get()->GetToNode() == pNode)
		{
			std::erase_if(decisions_, [spEdge](const std::shared_ptr<FSMGraphDecision>& spDecision)
			{
				return spDecision.get()->getOwnerEdge() == spEdge.get();
			});
		}
	});
	//ノードに繋がるエッジを削除
	std::erase_if(edges_,[pNode](const std::shared_ptr<FSMGraphEdge>& spEdge)
	{
		return spEdge.get()->GetFromNode() == pNode || spEdge.get()->GetToNode() == pNode;
	});
	//ノード削除
	std::erase_if(nodes_, [pNode](const std::shared_ptr<FSMGraphNode>& spNode)
	{
		return spNode.get() == pNode;
	});
	//index更新
	for (int index = 0; index < nodes_.size(); ++index)
	{
		nodes_[index].get()->SetNodeIndex(index);
	}
}

void FSMGraph::RemoveEdge(FSMGraphEdge* pEdge)
{
	//エッジにつけてる遷移条件を削除
	std::erase_if(decisions_, [pEdge](const std::shared_ptr<FSMGraphDecision>& spDecision)
	{
		return spDecision.get()->getOwnerEdge() == pEdge;
	});
	//エッジ
	std::erase_if(edges_, [pEdge](const std::shared_ptr<FSMGraphEdge>& spEdge)
	{
		return spEdge.get() == pEdge;
	});
	//index更新
	for (int index = 0; index < edges_.size(); ++index)
	{
		edges_[index].get()->SetEdgeIndex(index);
	}
}

void FSMGraph::RemoveDecision(FSMGraphDecision* pDecision)
{	;
	if (auto* pOwnerEdge = pDecision->getOwnerEdge())
	{
		pOwnerEdge->SetAttachedDecision(nullptr);
	}
	std::erase_if(decisions_, [pDecision](const std::shared_ptr<FSMGraphDecision>& spDecision)
	{
		return spDecision.get() == pDecision;
	});
	//index更新
	for (int index = 0; index < decisions_.size(); ++index)
	{
		decisions_[index].get()->SetDecisionIndex(index);
	}
}

void FSMGraph::SetDraggingDecision(FSMGraphDecision* pDecision)
{
	pDecision->SetIsDragging(true);
	pDraggingDecision_ = pDecision;
}

void FSMGraph::handleEvents()
{
	if (pSelectedEdge_)
	{
		addDecisionPanel_.AddDecision();
	}
	else
	{
		addNodePanel_.AddNode();
	}
	
	addEdgeBetweenNodes();
	onCursorClick();
	selectNode();
	selectEdge();
	selectDecision();
	dragNode();
	dragDecision();
	attachDecisionToEdge();
	removeSelectedNode();
	removeSelectedEdge();
	removeSelectedDecision();	
	saveFSMGraph();
	loadFSMGraph();
	selectAndLoadFSMGraphFile();
	compileGraphAndStartDebug();
	stopDebug();
}

void FSMGraph::addEdgeBetweenNodes()
{
	if (KeyE.pressed())
	{
		isAddingEdge_ = true;
	}
	else
	{
		isAddingEdge_ = false;
		pFromNodeForCreate = nullptr;
		pToNodeForCreate = nullptr;
	}

	if (isAddingEdge_)
	{
		if (MouseL.down())
		{
			for (auto& spNode : nodes_)
			{
				if (spNode->IsClicked())
				{
					pFromNodeForCreate = spNode.get();
				}
			}
		}
		if (MouseL.up() && pFromNodeForCreate)
		{
			for (auto& spNode : nodes_)
			{
				if (spNode->IsReleased())
				{
					pToNodeForCreate = spNode.get();
				}
			}
			if (!pToNodeForCreate)
			{
				pFromNodeForCreate = nullptr;
			}
		}
		if (pFromNodeForCreate && pToNodeForCreate && pFromNodeForCreate != pToNodeForCreate)
		{
			for (auto& spEdge : edges_)
			{
				if (spEdge->GetFromNode() == pFromNodeForCreate && spEdge->GetToNode() == pToNodeForCreate)
				{
					pFromNodeForCreate = nullptr;
					pToNodeForCreate = nullptr;
					return;
				}
			}
			auto spEdge = std::make_shared<FSMGraphEdge>(pFromNodeForCreate, pToNodeForCreate);
			edges_.push_back(spEdge);
			spEdge->SetEdgeIndex(edges_.size() - 1);
			pFromNodeForCreate = nullptr;
			pToNodeForCreate = nullptr;
		}
	}
}

void FSMGraph::onCursorClick()
{
	if (MouseL.down())
	{
		effect_.add<CursorEffect>(Cursor::Pos());
	}
}


bool FSMGraph::selectNode()
{
	bool isSelect = false;
	if (!isAddingEdge_)
	{
		if (MouseL.down())
		{
			for (auto& spNode : nodes_)
			{
				if (spNode->IsClicked())
				{
					if (pSelectedNode_)
					{
						pSelectedNode_->SetIsSelected(false);
						pSelectedNode_ = nullptr;
					}
					spNode->SetIsSelected(true);
					isSelect = true;
					pSelectedNode_ = spNode.get();
					return isSelect;
				}				
			}
			if (graphWindow.leftClicked())
			{
				if (pSelectedNode_)
				{
					pSelectedNode_->SetIsSelected(false);
					pSelectedNode_ = nullptr;
				}
			}

		}
	}
	return isSelect;
}

bool FSMGraph::selectEdge()
{
	bool isSelect = false;
	if (!isAddingEdge_)
	{
		if (MouseL.down())
		{
			for (auto& spEdge : edges_)
			{
				if (spEdge->IsClicked() && !pSelectedNode_)
				{
					if (pSelectedEdge_)
					{
						pSelectedEdge_->SetIsSelected(false);
						pSelectedEdge_ = nullptr;
					}
					spEdge->SetIsSelected(true);
					isSelect = true;
					pSelectedEdge_ = spEdge.get();
					return isSelect;
				}
			}
			if (graphWindow.leftClicked())
			{
				if (pSelectedEdge_)
				{
					pSelectedEdge_->SetIsSelected(false);
					pSelectedEdge_ = nullptr;
					if (pSelectedDecision_)
					{
						pSelectedDecision_->SetIsSelected(false);
						pSelectedDecision_ = nullptr;
					}
				}				
			}

		}
	}
	return isSelect;
}

bool FSMGraph::selectDecision()
{
	bool isSelect = false;
	if (!isAddingEdge_)
	{
		if (MouseL.down())
		{
			for (auto& spDecison : decisions_)
			{
				if (spDecison->GetIsActive() && spDecison->IsClicked())
				{
					if (pSelectedDecision_)
					{
						pSelectedDecision_->SetIsSelected(false);
						pSelectedDecision_ = nullptr;
					}
					spDecison->SetIsSelected(true);
					isSelect = true;
					pSelectedDecision_ = spDecison.get();
					return isSelect;
				}
			}
			if (propertyWindow.leftClicked())
			{
				if (pSelectedDecision_)
				{
					pSelectedDecision_->SetIsSelected(false);
					pSelectedDecision_ = nullptr;
				}
			}

		}
	}
	return isSelect;
}


void FSMGraph::dragNode()
{
	if (!isAddingEdge_)
	{
		if (MouseL.down())
		{
			for (auto& spNode : nodes_)
			{
				if (spNode->IsClicked())
				{
					spNode->SetIsDragging(true);
				}
			}
		}
		else if (MouseL.up())
		{
			for (auto& spNode : nodes_)
			{
				spNode->SetIsDragging(false);
			}
		}
		if (Cursor::DeltaF().lengthSq() > DBL_EPSILON)
		{
			for (auto& spNode : nodes_)
			{
				if (spNode->GetIsDragging())
				{
					spNode->SetPosition(spNode->GetPosition() + Cursor::DeltaF());
				}
			}
		}
	}	
}

void FSMGraph::dragDecision()
{
	if (!isAddingEdge_)
	{
		if (MouseL.down())
		{
			for (auto& spDecision : decisions_)
			{
				if (spDecision->IsClicked() && spDecision.get()->GetIsActive())
				{
					spDecision->SetIsDragging(true);
					pDraggingDecision_ = spDecision.get();
				}
			}
		}
		else if (MouseL.up())
		{
			for (auto& spDecision : decisions_)
			{
				spDecision->SetIsDragging(false);				
			}
			if (!canAttachDecision_)
			{
				pDraggingDecision_ = nullptr;
			}
		}
		if (Cursor::DeltaF().lengthSq() > DBL_EPSILON)
		{
			for (auto& spDecision : decisions_)
			{
				if (spDecision->GetIsDragging())
				{
					spDecision->SetPosition(spDecision->GetPosition() + Cursor::DeltaF());
				}
			}
		}
	}
}

void FSMGraph::attachDecisionToEdge()
{
	if (pSelectedEdge_ && pDraggingDecision_)
	{
		if (pSelectedEdge_->GetProperty().IsSlotAttached(pDraggingDecision_->GetDisplayNode()))
		{
			canAttachDecision_ = true;
		}
		else
		{
			canAttachDecision_ = false;
		}
		
		if (MouseL.up() && canAttachDecision_)
		{
			pDraggingDecision_->SetPosition(pSelectedEdge_->GetProperty().GetSlotPos());
			pSelectedEdge_->SetAttachedDecision(pDraggingDecision_);
			pDraggingDecision_->setOwnerEdge(pSelectedEdge_);
			pDraggingDecision_->SetIsAttached(true);
			pDraggingDecision_ = nullptr;
			canAttachDecision_ = false;
		}
	}
	else
	{
		canAttachDecision_ = false;
	}
}

void FSMGraph::removeSelectedNode()
{
	if (pSelectedNode_)
	{
		if (KeyDelete.down() && !isDebugging_)
		{
			RemoveNode(pSelectedNode_);
			pSelectedNode_ = nullptr;
		}
	}
}

void FSMGraph::removeSelectedEdge()
{
	if (pSelectedEdge_ && !pSelectedDecision_)
	{
		if (KeyDelete.down() &&!isDebugging_)
		{
			RemoveEdge(pSelectedEdge_);
			pSelectedEdge_ = nullptr;
		}
	}
}

void FSMGraph::removeSelectedDecision()
{
	if (pSelectedDecision_)
	{
		if (KeyDelete.down() && !isDebugging_)
		{
			RemoveDecision(pSelectedDecision_);
			pSelectedDecision_ = nullptr;
		}
	}
}



void FSMGraph::saveFSMGraph()
{
	//test
	/*if (KeyControl.pressed() && KeyS.down())
	{
		std::string testFileName = "FSMGraph/test2.fsmg";
		FSMGraphSaveLoad::Get()->SaveGraph(this, testFileName);
	}*/
	if (SimpleGUI::Button(U"Save", Vec2{ 1510, 60 }))
	{
		if (!filePathTe_.text.empty())
		{
			String saveFileName = filePathTe_.text;
			String saveFilePath = U"FSMGraph/" + saveFileName;
			FSMGraphSaveLoad::Get()->SaveGraph(this, saveFilePath.narrow());
		}
	}
}

void FSMGraph::loadFSMGraph()
{
	//test
	if (KeyControl.pressed() && KeyL.down())
	{
		std::string testFileName = "FSMGraph/test2.fsmg";
		FSMGraphSaveLoad::Get()->LoadGraph(this, testFileName);
	}
}

void FSMGraph::selectAndLoadFSMGraphFile()
{	
	if (SimpleGUI::Button(U"Open", Vec2{ 1415, 60 }))
	{
		fsmgFilePath_ = Dialog::OpenFile({ { U"Text file", { U"fsmg" } } });
		if (fsmgFilePath_.has_value())
		{
			std::filesystem::path filePath = fsmgFilePath_.value().narrow();
			fsmgFileName_ = Unicode::Widen(filePath.filename().string());
			filePathTe_.text = fsmgFileName_;
			FSMGraphSaveLoad::Get()->LoadGraph(this, fsmgFilePath_.value().narrow());
		}
	}
	SimpleGUI::TextBox(filePathTe_, Vec2{ 1210, 60 }, 200);

	/*if (!fsmgFileName_.empty())
	{
		SimpleGUI::TextBox(filePathTe_, Vec2{ 1210, 60 }, 200);		
	}*/
}

void FSMGraph::compileGraphAndStartDebug()
{
	if (SimpleGUI::Button(U"Debug開始", Vec2{ 585, 60 }))
	{
		if (nodes_.size() != 0 && pCurrentEnemy_)
		{
			pCompiledController_ = FSMCompiler::Get()->CompileFromGraph(this);
			if (pCompiledController_ == nullptr)
			{
				System::MessageBoxOK(U"コンパイル失敗");
				return;
			}
			setup.SetupAI(pCurrentEnemy_, pCompiledController_);
			setup.StartDebug(pCurrentEnemy_);
			isDebugging_ = true;
		}
		else if (nodes_.size() == 0)
		{
			System::MessageBoxOK( U"ノードが存在しません" );
		}
		else if (!pCurrentEnemy_)
		{
			System::MessageBoxOK(U"エネミーを選んでください");
		}
	}
}

void FSMGraph::stopDebug()
{
	if (SimpleGUI::Button(U"Debug停止", Vec2{ 730, 60 }))
	{
		setup.StopDebug();
		pCompiledController_ = nullptr;
		pCurrentEnemy_ = nullptr;
		isDebugging_ = false;
	}
}

void FSMGraph::updateDebugArea()
{
	//デルタ時間
	deltaTime = Scene::DeltaTime();
	//アプリ起動後の時間(ミリ秒単位)
	millisecTime = Time::GetMillisec();
	//セットアップでデバッグ機能が使える
	//setup.Update();
	//更新処理
	pManager->Update(deltaTime);

	//Component更新処理
	pCManager->Update();
	//designatedPosをpositionに適用する
	pManager->UpdatePosition();
}

void FSMGraph::drawDebugArea()
{
	//描画処理	
	pManager->UpdateRender(millisecTime);
	pManager->UpdateEffect();
}
