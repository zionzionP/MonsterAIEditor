#pragma once
#include "AIGraph/Editor/UI/FSMGraphSelectEnemyButton.h"

class FSMGraph;

enum class EnemyType
{
	Ghost = 0,
	SkeletonKnight,

	EnemyTypeMAX,
};

class FSMGrapSelectEnemyPanel
{
public:
	FSMGrapSelectEnemyPanel(FSMGraph* pOwner) :
		pOwnerGraph_(pOwner)
	{
		setupSelectEnemyButton();
	}
	~FSMGrapSelectEnemyPanel() = default;

	void Update();
	void Draw();

private:
	void setupSelectEnemyButton();
	void handleEvents();
	
	void selectEnemy();
	void geneEnemyWithoutAI();


	void drawSelectEnemyButton();
	
private:	
	FSMGraph* pOwnerGraph_ = nullptr;
	Rect selectEnemyPanel_{ 5, 655, 575, 240 };
	Vec2 panalTL = { 5,655 };
	double buttonSize = 96;
	int32 selectedEnemyIndex_ = 0;
	std::vector<FSMGraphSelectEnemyButton> selectEnemyButtons_;
	const Font font{ FontMethod::MSDF, 48 };

	//描画用テキスチャー	
	String ghostTexturePath = U"Assets/pipo-charachip019.png";
	String skeletonKnightTexturePath = U"Assets/skeleton-knight.png";
	std::vector<String> texturePaths_ = { ghostTexturePath, skeletonKnightTexturePath };

};
