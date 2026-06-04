#pragma once
#include "LevelData/LevelData.h"

class DataManager
{
public:
	DECLARE_SINGLE(DataManager);

	void Init();
	void Clear();

	void SaveMapData(const std::wstring& filename, const std::vector<ObjectSpawnData>& mapDataList);

	void LoadMapData(const std::wstring& fileName);

	const MapData& GetCurrentMapData() { return _currentMap; };


private:
	MapData _currentMap;
};