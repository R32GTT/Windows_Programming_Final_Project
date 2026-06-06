#include "pch.h"
#include "Floor.h"
#include "../LevelData/LevelData.h"


Floor::~Floor()
{
}

void Floor::Init()
{
}

void Floor::Update()
{
}

void Floor::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
}

void Floor::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
}

void Floor::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
}
