#pragma once
#include "Level/Level.h"
#include "Level/LevelManager.h"
class TitleLevel : public Level
{
public:
	TitleLevel(){}
	~TitleLevel() = default;

	void Start() override;
	void Update() override;

private:
	RenderTexture renderTexture{ 640, 480 };
	Font font{ FontMethod::MSDF, 48 };
	bool isInstructionOn = false;
};
