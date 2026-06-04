#include "pch.h"
#include "PlayScene.h"
#include "Managers.h"

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	GET_SINGLE(DataManager)->LoadMapData(L"Map1.json");

	const MapData& mapData = GET_SINGLE(DataManager)->GetCurrentMapData();

	Super::BuildMapFromData(mapData);
}

void PlayScene::Update()
{
	Super::Update();
}

void PlayScene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Super::Render(renderTarget, alpha);
}
