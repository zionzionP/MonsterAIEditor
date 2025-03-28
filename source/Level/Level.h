#pragma once

class LevelManager;
//ゲームレベルの基底クラス
class Level
{
public:
	Level(){}
	virtual ~Level() = default;

	void SetLevelCleared(bool in) { isLevelCleared = in; }
	virtual void Start(){}
	virtual void Update() {}
	virtual void CheckLevelChange() {}
	virtual void Exit() {}


protected:
	bool isUpdate = true;
	bool isLevelCleared = false;
};
