#include "pch.h"
#include "EndPoint.h"
#include "../LevelData/LevelData.h"

EndPoint::EndPoint()
{
	type = OBJECTTYPE::ENDPOINT;
	layer = Layers::WALL;
}

EndPoint::~EndPoint()
{
}

void EndPoint::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.textData;
}

void EndPoint::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
	spawnData.spriteName;
}
