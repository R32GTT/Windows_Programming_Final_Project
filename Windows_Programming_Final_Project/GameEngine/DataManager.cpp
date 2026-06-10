#include "pch.h"
#include "Managers.h"
#include "Scene/Scene.h"
#include "Utility/EnumUtils.h"
#include "FileBase/json.hpp"


void DataManager::Init()
{
}

void DataManager::Clear()
{
	_currentMap.objects.clear();
}

void DataManager::SaveMapData(const std::wstring& filename, const std::vector<ObjectSpawnData>& mapDataList)
{
    json mapJson;
    
    mapJson["objects"] = SerializeMapObjects(mapDataList);

    GET_SINGLE(FileManager)->SaveMapJson(filename, mapJson);

}

void DataManager::SaveChaperData(const std::wstring& filename, const ChapterData& chapterData)
{
    json chapterJson;

    chapterJson["ChapterID"] = chapterData.chapterID;

    // 맵의 '데이터'가 아니라 '파일 이름'만 배열로 저장
    json mapsArray = json::array();
    for (const std::wstring& mapFileName : chapterData.mapFileNames)
    {
        // nlohmann/json은 보통 std::string을 사용하므로 변환 필요
        mapsArray.push_back(WStrToStr(mapFileName));
    }

    chapterJson["MapFiles"] = mapsArray;

    GET_SINGLE(FileManager)->SaveMapJson(filename, chapterJson);
}

//성공적으로 맵을 이동했다면 true, 더 이상 맵이 없어서 챕터가 끝났다면 false반환으로 변경
bool DataManager::GoToNextMap(std::string mapInfo)
{
    int nextIdx = std::stoi(mapInfo);

    if (nextIdx < _currentChapter.mapFileNames.size())
    {
        _currentMapIdx = nextIdx;

        if (_mapCache.find(_currentMapIdx) != _mapCache.end())
        {
            const MapData& nextMapData = _mapCache[_currentMapIdx];

            Scene* curScene = GET_SINGLE(SceneManager)->GetCurrentScene();
            if (curScene != nullptr)
            {
                curScene->Clear();
                curScene->BuildMapFromData(nextMapData);
            }

            // [성공] 성공적으로 다음 맵으로 이동함
            return true;
        }

        return false;
    }
    else
    {
        // [챕터 종료] 더 이상 갈 수 있는 다음 맵 인덱스가 없음! (예: 3개 중 4번째 맵을 요구함)
        return false;
    }
}


bool DataManager::GoToNextChapter()
{
    _currentChapterIdx++;

    //다음 챕터가 존재하는 경우
    if (_currentChapterIdx <= _maxChapterCount)
    {
        std::wstring nextChapterFileName = L"Chapter" + std::to_wstring(_currentChapterIdx) + L".json";

        LoadChaperData(nextChapterFileName);

        //챕터 이동 성공
        return true;

    }
    //모든 챕터를 클리어한 경우
    else {
        //EndGame을 직접 호출하지 않는다
        //더 이상 챕터가 없음을 알린다
        return false;
        //EndGame();
    }

}

void DataManager::EndGame()
{
    //최종 클리어 처리(결과창 띄우기, 메인 타이틀 이동 등)
    Scene* curScene = GET_SINGLE(SceneManager)->GetCurrentScene();

    if (curScene != nullptr)
    {
        //맵에 있는 적이다 투사체 등을 모두 정리
        curScene->Clear();
    }
   
}


void DataManager::LoadMapData(const std::wstring& fileName)
{
    _currentMap.objects.clear();
    json mapJson = GET_SINGLE(FileManager)->LoadMapJson(fileName);

    if (mapJson.is_null() || mapJson.empty()) return;

    DeserializeMapObjects(mapJson, _currentMap);
}

void DataManager::LoadChaperData(const std::wstring& fileName)
{
    _mapCache.clear();
    _currentChapter.mapFileNames.clear();

    json chapterJson = GET_SINGLE(FileManager)->LoadMapJson(fileName);
    if (chapterJson.is_null() || chapterJson.empty()) return;

    if (chapterJson.contains("ChapterID")) _currentChapter.chapterID = chapterJson["ChapterID"];

    if (chapterJson.contains("MapFiles"))
    {
        int mapIdx = 0;
        for (const auto& mapFileName : chapterJson["MapFiles"])
        {
            std::wstring wMapName = StrToWStr(mapFileName.get<std::string>());
            _currentChapter.mapFileNames.push_back(wMapName);

            // 개별 맵 데이터를 로드해서 메모리에 적재
            json mapJson = GET_SINGLE(FileManager)->LoadMapJson(wMapName);
            MapData mData{};
            mData.mapIDX = mapIdx;
            DeserializeMapObjects(mapJson, mData);

            _mapCache[mapIdx] = mData;
            mapIdx++;
        }
    }

    _currentMapIdx = 0;
    Scene* currentScene = GET_SINGLE(SceneManager)->GetCurrentScene();
    if (currentScene != nullptr && !_mapCache.empty())
    {
        currentScene->Clear();
        currentScene->BuildMapFromData(_mapCache[_currentMapIdx]);
    }
}

json DataManager::SerializeMapObjects(const std::vector<ObjectSpawnData>& mapDataList)
{
    json objectsArray = json::array();
    for (const ObjectSpawnData& data : mapDataList)
    {
        json objJson;
        objJson["Id"] = data.fileID;
        objJson["ObjectType"] = ObjectTypeToString(data.objType);
        objJson["Layer"] = LayerToString(data.layers);
        objJson["x"] = data.pos.x;
        objJson["y"] = data.pos.y;
        objJson["halfSize_x"] = data.halfSize.x;
        objJson["halfSize_y"] = data.halfSize.y;
        objJson["SpriteName"] = WStrToStr(data.spriteName);
        objJson["EnemyType"] = EnemyTypeToString(data.enemyType);
        objJson["WallRectL"] = data.WallCoords.left;
        objJson["WallRectR"] = data.WallCoords.right;
        objJson["WallRectT"] = data.WallCoords.top;
        objJson["WallRectB"] = data.WallCoords.bottom;
        objJson["WeaponType"] = WeaponTypeToString(data.weaponType);
        if (!data.textData.empty()) objJson["TextData"] = data.textData;

        objectsArray.push_back(objJson);
    }
    return objectsArray;
}

void DataManager::DeserializeMapObjects(const json& mapJson, MapData& outMapData)
{
    if (!mapJson.contains("objects")) return;

    for (const auto& objJson : mapJson["objects"])
    {
        ObjectSpawnData data{};
        if (objJson.contains("Id")) data.fileID = objJson["Id"];
        if (objJson.contains("ObjectType")) data.objType = StringToObjectType(objJson["ObjectType"]);
        if (objJson.contains("Layer")) data.layers = StringToLayer(objJson["Layer"]);
        if (objJson.contains("x")) data.pos.x = objJson["x"];
        if (objJson.contains("y")) data.pos.y = objJson["y"];
        if (objJson.contains("halfSize_x")) data.halfSize.x = objJson["halfSize_x"];
        if (objJson.contains("halfSize_y")) data.halfSize.y = objJson["halfSize_y"];
        if (objJson.contains("SpriteName")) data.spriteName = StrToWStr(objJson["SpriteName"].get<std::string>());
        if (objJson.contains("EnemyType")) data.enemyType = StringToEnemyType(objJson["EnemyType"]);
        if (objJson.contains("WallRectL")) data.WallCoords.left = objJson["WallRectL"];
        if (objJson.contains("WallRectR")) data.WallCoords.right = objJson["WallRectR"];
        if (objJson.contains("WallRectT")) data.WallCoords.top = objJson["WallRectT"];
        if (objJson.contains("WallRectB")) data.WallCoords.bottom = objJson["WallRectB"];
        if (objJson.contains("WeaponType")) data.weaponType = StringToWeaponType(objJson["WeaponType"]);
        if (objJson.contains("TextData")) data.textData = objJson["TextData"].get<std::string>();

        outMapData.objects.push_back(data);
    }
}