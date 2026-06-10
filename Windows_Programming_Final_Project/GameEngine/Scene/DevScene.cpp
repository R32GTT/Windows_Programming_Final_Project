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
	/*Player* pp = new Player();
	pp->SetPos(Vec2<float>(400.0f, 300.0f));
	
	AddObject(pp);

	_cam.SetOwner(pp);*/

	OutputDebugString(L"DevScene Init 실행됨\n");

	Super::Init();

	Vec2<float> basePos = Vec2<float>(300.0f, 300.0f);

	if (Super::GetPlayer() != nullptr)
	{
		basePos = Super::GetPlayer()->GetPos();
	}

	std::vector<Vec2<float>> weaponOffSets = {

		Vec2<float>(-200.0f, -100.0f),
		Vec2<float>(-100.0f, -100.0f),
		Vec2<float>(0.0f, -100.0f),
		Vec2<float>(100.0f, -100.0f),
		Vec2<float>(200.0f, -100.0f),

		Vec2<float>(-200.0f, 100.0f),
		Vec2<float>(-100.0f, 100.0f),
		Vec2<float>(0.0f, 100.0f),
		Vec2<float>(100.0f, 100.0f),
		Vec2<float>(200.0f, 100.0f),
	};

	for (const Vec2<float>& offset : weaponOffSets)
	{
		Weapon* weapon = new Weapon();

		Vec2<float> spawnPos = basePos + offset;

		weapon->SetPos(spawnPos);
		weapon->SavePrevPos();
		weapon->Init();
		AddObject(weapon);

	}

	if (Super::GetPlayer() != nullptr)
	{
		Super::SetCamOwner(Super::GetPlayer());
	}
}

void DevScene::Update()
{
	Super::Update();
}

void DevScene::Render(ID2D1RenderTarget* renderTarget,float alpha)
{
	Super::Render(renderTarget, alpha);
}
