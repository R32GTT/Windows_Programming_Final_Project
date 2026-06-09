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

	//반환형을 bool로 변경함
	bool GoToNextMap(std::string mapInfo);

	//새로 추가된 함수
	//챕터 넘어가는 함수와 종료함수 추가
	//챕터 넘어가는 함수도 bool 반환으로
	bool GoToNextChapter();
	void EndGame();

	void LoadMapData(const std::wstring& fileName);
	void LoadChaperData(const std::wstring& fileName);

	const MapData& GetCurrentMapData() { return _currentMap; };

	//추가된 것
	int GetCurrentMapIdx() const { return _currentMapIdx; }

private:

	json SerializeMapObjects(const std::vector<ObjectSpawnData>& mapDataList);
	void DeserializeMapObjects(const json& mapJson, MapData& outMapData);

	MapData _currentMap;
	ChaperData _currentChapter;
	int _currentMapIdx = 0;

	//새로 추가된 변수
	//시작 챕터 번호와 총 챕터 개수 번호변수
	int _currentChapterIdx = 1;
	int _maxChapterCount = 3;

};