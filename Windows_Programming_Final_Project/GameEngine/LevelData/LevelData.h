#pragma once
#include "pch.h"
#include "Enums.h"

struct ObjectSpawnData
{
	unsigned int fileID;
	OBJECTTYPE objType;
	Layers layers;

	Vec2F pos;
	Vec2F halfSize;

	D2D1_RECT_F WallCoords{};

	EnemyType enemyType = EnemyType::NORMAL;
	WPTYPE weaponType = WPTYPE::FIST;

	std::wstring spriteName{ L"" };

	std::string textData = "";
};

struct MapData
{
	int mapIDX;
	std::wstring mapName;

	std::vector<ObjectSpawnData> objects;
};

struct ChapterData
{
	int chapterID;
	std::wstring chapterName;

	std::vector<std::wstring> mapFileNames; // 파일명 목록만 저장
};