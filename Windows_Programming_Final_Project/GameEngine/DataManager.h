#pragma once
#include "LevelData/LevelData.h"
#include "FileBase/json.hpp"

using json = nlohmann::json;
class DataManager
{
public:
	DECLARE_SINGLE(DataManager);

	void Init();
	void Clear();

	void SaveMapData(const std::wstring& filename, const std::vector<ObjectSpawnData>& mapDataList);

	void GoToNextMap(std::string mapInfo);

	void LoadMapData(const std::wstring& fileName);
	void LoadChaperData(const std::wstring& fileName);

	const MapData& GetCurrentMapData() { return _currentMap; };

private:

	json SerializeMapObjects(const std::vector<ObjectSpawnData>& mapDataList);
	void DeserializeMapObjects(const json& mapJson, MapData& outMapData);

	MapData _currentMap;
	ChaperData _currentChapter;
	int _currentMapIdx = 0;
};