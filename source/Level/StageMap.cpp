#include "Level/StageMap.h"
std::vector<int> StageMap::LoadCSV(FilePathView i_path)
{
	CSV csv{ i_path };

	const int yCount = csv.rows();
	const int xCount = csv.columns(0);

	std::vector<int> map;

	for (int y = 0; y < yCount; ++y)
	{
		for (int x = 0; x < xCount; ++x)
		{
			// 指定した行・列の値を読み込む
			map.push_back(csv.get<int>(y, x));
		}
	}
	return map;
}

void StageMap::LoadMap()
{
	mapLayers_.emplace(LayerName::LOOK_STAGE1, LoadCSV(U"Assets/LayerLookStage1.csv"));
	mapLayers_.emplace(LayerName::LOOK_STAGE1_OVER, LoadCSV(U"Assets/LayerLookStage1LayerOver.csv"));
	mapLayers_.emplace(LayerName::LOOK_STAGE2_LAYER1, LoadCSV(U"Assets/LayerLookStage2Layer1.csv"));
	mapLayers_.emplace(LayerName::LOOK_STAGE2_LAYER2, LoadCSV(U"Assets/LayerLookStage2Layer2.csv"));
	mapLayers_.emplace(LayerName::LOOK_STAGE2_OVER, LoadCSV(U"Assets/LayerLookStage2LayerOver.csv"));
	mapLayers_.emplace(LayerName::LOOK_STAGE3_LAYER1, LoadCSV(U"Assets/LayerLookStage3Layer1.csv"));
	mapLayers_.emplace(LayerName::LOOK_STAGE3_LAYER2, LoadCSV(U"Assets/LayerLookStage3Layer2.csv"));
	mapLayers_.emplace(LayerName::LOOK_STAGE3_OVER, LoadCSV(U"Assets/LayerLookStage3LayerOver.csv"));
	mapLayers_.emplace(LayerName::MOVEABLE_STAGE1, LoadCSV(U"Assets/LayerMoveableStage1.csv"));
	mapLayers_.emplace(LayerName::MOVEABLE_STAGE2, LoadCSV(U"Assets/LayerMoveableStage2.csv"));
	mapLayers_.emplace(LayerName::MOVEABLE_STAGE3, LoadCSV(U"Assets/LayerMoveableStage3.csv"));
	isMapLoaded = true;
}

void StageMap::ClearMap()
{
	isMapLoaded = false;
	mapLayers_.clear();
	auto& look1 = mapLayers_[10];
}

void StageMap::UpdateMapRender(LayerName i_name)
{
	if (isMapLoaded)
	{
		for (int x = 0; x < mapWidth_; ++x)
		{
			for (int y = 0; y < mapHeight_; ++y)
			{				
				if (mapLayers_[i_name][x + (y * mapWidth_)] > 0)
				{
					Point pos{ (x * Common::CHIPSIZE), (y * Common::CHIPSIZE) };
					GetMapChip(mapLayers_[i_name][x + (y * mapWidth_)]).draw(pos);
				}
				else if (mapLayers_[i_name][x + (y * mapWidth_)] < 0)
				{
					Point pos{ (x * Common::CHIPSIZE), (y * Common::CHIPSIZE) };
					GetMapChipHalf(-mapLayers_[i_name][x + (y * mapWidth_)]).draw(pos);
				}
			}

		}
	}
}

void StageMap::UpdateOverMapRender(LayerName i_name)
{
	if (isMapLoaded)
	{
		for (int x = 0; x < mapWidth_; ++x)
		{
			for (int y = 0; y < mapHeight_; ++y)
			{
				if (mapLayers_[i_name][x + (y * mapWidth_)] > 0)
				{
					Point pos{ (x * Common::CHIPSIZE), (y * Common::CHIPSIZE) };
					GetMapChipHalf(mapLayers_[i_name][x + (y * mapWidth_)]).draw(pos);
				}
			}

		}
	}
}

