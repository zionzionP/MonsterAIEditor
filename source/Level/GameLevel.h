#pragma once
#include "Level/Level.h"
#include "Level/LevelManager.h"
#include "Level/Setup.h"
class GameLevel : public Level
{
public:
	GameLevel() {}
	virtual ~GameLevel() = default;

	virtual void Start() override;
	virtual void Update() override;

	virtual void CheckLevelChange() override;

	virtual void Exit() override;
	void UpdatePause();
	void UpdateRenderWarpGate(const Vec2& i_pos);

	void UpdateRenderArrowToGate(const Vec2& i_pos);

protected:
	RenderTexture renderTexture{ 640, 480 };
	Font font{ FontMethod::MSDF, 48 };
	double deltaTime = 0;
	uint64 millisecTime = 0;
	ActorManager* pManager = ActorManager::Get();
	ComponentFactoryManager* pCManager = ComponentFactoryManager::Get();
	Setup setup;
	bool isPaused = false;
	Camera2D camera;
	const Texture warpGateEffectTexture{ U"Assets/pipo-mapeffect015_192.png" };
	const Texture arrowEffectTexture{ U"Assets/pipo-Text_Pause005.png" };
	int effectIndex = 0;
	int arrowEffectIndex = 0;
};
