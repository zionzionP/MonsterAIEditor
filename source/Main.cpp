#include <Siv3D.hpp>
#include "Actor/ActorManager.h"
#include "Core/Common.h"
#include "Core/Random.h"
#include "Level/LevelManager.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


void Main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Window::Resize(Common::WindowWidth, Common::WindowHeight);

	LevelManager* pLManager = LevelManager::Get();
	pLManager->SetLevels();

	int FPS = 60;
	Stopwatch sw;
	sw.start();

	while (System::Update())
	{		
		pLManager->Update();

		while (sw.msF() < 1000.0 / FPS);
		sw.restart();
	}
	//Exit flow
	ActorManager::Get()->ClearActor();

	_CrtDumpMemoryLeaks();
	
}
