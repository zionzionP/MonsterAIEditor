#include "Level/EditorLevel.h"
#include "AIGraph/Editor/Node/FSMGraphNode.h"
#include "AIGraph/Editor/FSMCompiler.h"

void EditorLevel::Start()
{
	Window::Resize(Common::EditorWindowWidth, Common::EditorWindowHeight);
	ComponentFactoryManager::Get()->AddFactory(COLLISION, DBG_NEW CollisionComponentFactory);
}
void EditorLevel::Update()
{
	// 現在のマウスカーソル座標を表示する(dbg)
	//ClearPrint();
	//Print << Cursor::Pos();
	

	renderTexture.clear(Color{30,30,30}).draw();
	font(U"AIグラフエディター").draw(30, Vec2{ 30, 5 }, Palette::White);	

	if (currentFSMGraph_)
	{
		currentFSMGraph_->Update();		
	}
	else if	(KeySpace.down())
	{
		createNewFsmGraph();
	}

	if (currentFSMGraph_)
	{
		currentFSMGraph_->Draw();
	}

}


void EditorLevel::Exit()
{
	if (currentFSMGraph_)
	{
		delete currentFSMGraph_;
		currentFSMGraph_ = nullptr;
	}
	ActorManager::Get()->ClearActor();
	Window::Resize(Common::WindowWidth, Common::WindowHeight);
}

FSMGraph* EditorLevel::createNewFsmGraph()
{
	currentFSMGraph_ = DBG_NEW FSMGraph();
	currentFSMGraph_->Initialize();

	return currentFSMGraph_;
}



