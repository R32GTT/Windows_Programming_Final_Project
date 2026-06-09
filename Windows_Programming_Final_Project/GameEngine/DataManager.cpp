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

void DataManager::GoToNextMap(std::string mapInfo)
{
    int nextIdx = std::stoi(mapInfo);

    if (nextIdx < _currentChapter.maps.size())
    {
        _currentMapIdx = nextIdx;

        // 다음 맵 데이터를 가져옴
        const MapData& nextMapData = _currentChapter.maps[_currentMapIdx];

        // 현재 씬(PlayScene)의 맵을 갈아끼움
        Scene* curScene = GET_SINGLE(SceneManager)->GetCurrentScene();
        if (curScene != nullptr)
        {
           //TODO
            curScene->Clear();
            curScene->BuildMapFromData(nextMapData);


        }
    }
    else
    {
        // 챕터 클리어 처리 (결과창 띄우기, 메인화면 이동 등)
    }
}

void DataManager::GoToNextChapter()
{
    _currentChapterIdx++;

    //다음 챕터가 존재하는 경우
    if (_currentChapterIdx <= _maxChapterCount)
    {
        std::wstring nextChapterFileName = L"Chapter" + std::to_wstring(_currentChapterIdx) + L".json";

        LoadChaperData(nextChapterFileName);
    }
    //모든 챕터를 클리어한 경우
    else {
        EndGame();
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
    json chapterJson = GET_SINGLE(FileManager)->LoadMapJson(fileName);
    if (chapterJson.is_null() || chapterJson.empty()) return;

    _currentChapter.maps.clear();
    if (chapterJson.contains("ChapterID")) _currentChapter.chapterID = chapterJson["ChapterID"];

    if (chapterJson.contains("Maps"))
    {
        for (const auto& mapJson : chapterJson["Maps"])
        {
            MapData mData{};
            if (mapJson.contains("MapIDX")) mData.mapIDX = mapJson["MapIDX"];

            // 여기서 헬퍼 함수를 재사용해서 깔끔하게 처리!
            DeserializeMapObjects(mapJson, mData);

            _currentChapter.maps.push_back(mData);
        }
    }

    _currentMapIdx = 0;
    Scene* currentScene = GET_SINGLE(SceneManager)->GetCurrentScene();
    if (currentScene != nullptr && !_currentChapter.maps.empty())
    {
        currentScene->Clear();
        currentScene->BuildMapFromData(_currentChapter.maps[_currentMapIdx]);
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