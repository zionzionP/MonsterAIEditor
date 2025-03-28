#pragma once
#include "Level/Level.h"
#include "Level/LevelManager.h"
#include "Level/Setup.h"
class DebugLevel : public Level
{
public:
	DebugLevel() {}
	~DebugLevel() = default;

	void Start() override;
	void Update() override;

	void Exit() override;

private:
	RenderTexture renderTexture{ 640, 480 };
	Font font{ FontMethod::MSDF, 48 };
	double deltaTime = 0;
	uint64 millisecTime = 0;
	ActorManager* pManager = ActorManager::Get();	
	ComponentFactoryManager* pCManager = ComponentFactoryManager::Get();
	Setup setup;
	bool isPaused = false;
	Camera2D camera{ Scene::Center() ,2.0};
};
