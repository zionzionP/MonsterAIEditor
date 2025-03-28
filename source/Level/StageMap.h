#pragma once
#include <Siv3D.hpp>
#include "Core/Common.h"
#include <unordered_map>

enum LayerName
{
	MOVEABLE_STAGE1 = 0,
	MOVEABLE_STAGE2 = 1,
	MOVEABLE_STAGE3 = 2,

	LOOK_STAGE1 = 10,
	LOOK_STAGE1_OVER = 19,
	LOOK_STAGE2_LAYER1 = 20,
	LOOK_STAGE2_LAYER2 = 21,
	LOOK_STAGE2_OVER = 29,
	LOOK_STAGE3_LAYER1 = 30,
	LOOK_STAGE3_LAYER2 = 31,
	LOOK_STAGE3_OVER = 39,


};
class StageMap
{
public:
	StageMap() {};
	~StageMap() = default;

	std::unordered_map<int, std::vector<int>>& GetMapLayers() { return mapLayers_; }
	const int GetWidth() const { return mapWidth_; }
	const int GetHeight() const { return mapHeight_; }
	//CSVからマップを読み込む
	void LoadMap();
	//読み込んだマップをクリア
	void ClearMap();
	//マップの描画処理
	void UpdateMapRender(LayerName i_name);
	void UpdateOverMapRender(LayerName i_name);
	std::vector<int> LoadCSV(FilePathView i_path);

	//ベース画像から必要なマップチップを切り出す
	TextureRegion GetMapChip(int chipIndex) const
	{
		                 //Asset画像の一行には8枚のチップ
		int x = ((chipIndex % 8) * Common::CHIPSIZE);
		int y = ((chipIndex / 8) * Common::CHIPSIZE);
		return mapTexture_(x, y, Common::CHIPSIZE);
	}

	//マップチップの上半分だけを切り出す
	TextureRegion GetMapChipHalf(int chipIndex) const
	{
		//Asset画像の一行には8枚のチップ
		int x = ((chipIndex % 8) * Common::CHIPSIZE);
		int y = ((chipIndex / 8) * Common::CHIPSIZE);
		return mapTexture_(x, y, Common::CHIPSIZE,Common::CHIPSIZE*0.5);
	}

private:
	int mapWidth_ = 20;
	int mapHeight_ = 15;
	Texture mapTexture_{ U"Assets/[Base]BaseChip_pipo.png" };
	
	//マップレイヤー
	std::unordered_map<int , std::vector<int>> mapLayers_;
	bool isMapLoaded = false;
};
