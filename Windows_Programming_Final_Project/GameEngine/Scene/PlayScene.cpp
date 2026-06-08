#include "pch.h"
#include "PlayScene.h"
#include "Managers.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	GET_SINGLE(DataManager)->LoadMapData(L"TestMap1.json");

	const MapData& mapData = GET_SINGLE(DataManager)->GetCurrentMapData();

	Super::BuildMapFromData(mapData);

	Super::Init();
}

void PlayScene::Update()
{
	Super::Update();
}

void PlayScene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Super::Render(renderTarget, alpha);
}
