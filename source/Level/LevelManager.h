#pragma once
#include "Core/Singleton.h"
#include "Level/Level.h"
#include "Level/TitleLevel.h"
#include "Level/DebugLevel.h"
#include "Level/GameLevel.h"
#include "Level/EditorLevel.h"
#include <unordered_map>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

class Level;
class GameLevelStage1;

enum LevelName
{
	TITLE_LEVEL = 0,
	GAME_LEVEL = 1,
	DEBUG_LEVEL = 2,
	GAME_LEVEL_STAGE1 = 3,
	GAME_LEVEL_STAGE2 = 4,
	GAME_LEVEL_STAGE3 = 5,

	EDITOR_LEVEL = 10,
};

class LevelManager : public Singleton<LevelManager>
{
public:
	LevelManager(){}
	~LevelManager() = default;

	const int GetPlayerHP() const { return playerHP_; }
	void SetPlayerHP(int in){ playerHP_ = in; }
	typedef std::unordered_map<int, std::unique_ptr<Level>> LevelMap;
	//レベルのインスタンスを作成
	void SetLevels();
	//レベルの更新処理
	void Update();
	//レベルの遷移
	void ChangeLevel(LevelName i_name);	

private:
	LevelMap levelMap_;
	Level* pCurrentLevel_ = nullptr;
	//プレイヤーのHPを引き継ぐため、HPを記録する
	int playerHP_ = 0;

	
};
