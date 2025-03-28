#include "Level/LevelManager.h"
#include "Level/GameLevelStage1.h"
#include "Level/GameLevelStage2.h"
#include "Level/GameLevelStage3.h"

void LevelManager::SetLevels()
{
	levelMap_.emplace(LevelName::TITLE_LEVEL, DBG_NEW TitleLevel);
	levelMap_.emplace(LevelName::GAME_LEVEL, DBG_NEW GameLevel);
	levelMap_.emplace(LevelName::DEBUG_LEVEL, DBG_NEW DebugLevel);
	levelMap_.emplace(LevelName::GAME_LEVEL_STAGE1, DBG_NEW GameLevelStage1);
	levelMap_.emplace(LevelName::GAME_LEVEL_STAGE2, DBG_NEW GameLevelStage2);
	levelMap_.emplace(LevelName::GAME_LEVEL_STAGE3, DBG_NEW GameLevelStage3);
	levelMap_.emplace(LevelName::EDITOR_LEVEL, DBG_NEW EditorLevel);

	pCurrentLevel_ = levelMap_[TITLE_LEVEL].get();
}

void LevelManager::Update()
{
	pCurrentLevel_->Update();
	pCurrentLevel_->CheckLevelChange();
}

void LevelManager::ChangeLevel(LevelName i_name)
{
	pCurrentLevel_->Exit();
	pCurrentLevel_ = levelMap_[i_name].get();
	pCurrentLevel_->Start();
}
