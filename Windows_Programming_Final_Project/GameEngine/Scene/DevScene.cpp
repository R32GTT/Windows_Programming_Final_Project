#include "pch.h"
#include "DevScene.h"
#include "Managers.h"
#include "GameObject.h"
#include "GameEngine.h"

DevScene::DevScene()
{
}

DevScene::~DevScene()
{
}

void DevScene::Init()
{
	Player* pp = new Player();
	pp->SetPos(Vec2<float>(400.0f, 300.0f));
	
	AddObject(pp);
}

void DevScene::Update()
{
	Super::Update();

	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
}

void DevScene::Render(HDC hdc)
{
	Super::Render(hdc);
}
