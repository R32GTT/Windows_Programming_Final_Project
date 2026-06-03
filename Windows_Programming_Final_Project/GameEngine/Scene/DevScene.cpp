#include "pch.h"
#include "DevScene.h"
#include "Managers.h"
#include "GameObject.h"
#include "GameEngine.h"
#include "../Objects/Objects.h"


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

	_cam.SetOwner(pp);
}

void DevScene::Update()
{
	Super::Update();
}

void DevScene::Render(ID2D1RenderTarget* renderTarget,float alpha)
{
	Super::Render(renderTarget, alpha);
}
