#include "pch.h"
#include "DataManager.h"
#include "FileManager.h"
#include "Utility/EnumUtils.h"

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
    mapJson["objects"] = json::array();

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

        //objJson["SpriteName"] = data.spriteName;

        objJson["EnemyType"] = EnemyTypeToString(data.enemyType);

        objJson["WallRectL"] = data.WallCoords.left;
        objJson["WallRectR"] = data.WallCoords.right;
        objJson["WallRectT"] = data.WallCoords.top;
        objJson["WallRectB"] = data.WallCoords.bottom;
        
        mapJson["objects"].push_back(objJson);
    }

    // 2. FileManager를 통해 디스크에 씁니다.
    // (이전에 FileManager에 추가해둔 SaveMapJson 함수 사용)
    GET_SINGLE(FileManager)->SaveMapJson(filename, mapJson);

}

void DataManager::LoadMapData(const std::wstring& fileName)
{
    _currentMap.mapName = fileName;
    _currentMap.objects.clear();

    json mapJson = GET_SINGLE(FileManager)->LoadMapJson(fileName);


    if (mapJson.contains("objects"))
    {
        for (const auto& objJson : mapJson["objects"])
        {
            ObjectSpawnData data;

            if (objJson.contains("Id")) data.fileID = objJson["Id"];
            if (objJson.contains("ObjectType")) data.objType = StringToObjectType(objJson["ObjectType"]);
            if (objJson.contains("Layer")) data.layers = StringToLayer(objJson["Layer"]);

            if (objJson.contains("x")) data.pos.x = objJson["x"];
            if (objJson.contains("y")) data.pos.y = objJson["y"];
            if (objJson.contains("halfSize_x")) data.halfSize.x = objJson["halfSize_x"];
            if (objJson.contains("halfSize_y")) data.halfSize.y = objJson["halfSize_y"];

            if (objJson.contains("EnemyType")) data.enemyType = StringToEnemyType(objJson["EnemyType"]);

            if (objJson.contains("WallRectL")) data.WallCoords.left = objJson["WallRectL"];
            if (objJson.contains("WallRectR")) data.WallCoords.right = objJson["WallRectR"];
            if (objJson.contains("WallRectT")) data.WallCoords.top = objJson["WallRectT"];
            if (objJson.contains("WallRectB")) data.WallCoords.bottom = objJson["WallRectB"];

            _currentMap.objects.push_back(data);
        }
    }
}
