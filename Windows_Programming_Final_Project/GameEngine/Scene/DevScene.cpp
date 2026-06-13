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
	Super::Init();

	Vec2<float> basePos = Vec2<float>(300.0f, 300.0f);

	if (Super::GetPlayer() != nullptr)
	{
		basePos = Super::GetPlayer()->GetPos();
	}

	
	if (Super::GetPlayer() != nullptr)
	{
		Super::SetCamOwner(Super::GetPlayer());
	}
}

void DevScene::Update()
{
	Super::Update();
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F5))
	{
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::EDITSCENE, L"TestMap1.json");
	}

	//추가된 코드: F2를 누르면 PlayScene으로 이동
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F2))
	{
		GET_SINGLE(SceneManager)->SetCurrentCombo(0);
		GET_SINGLE(SceneManager)->SetComboTimer(0.0f);

		GET_SINGLE(SceneManager)->SetTotalScore(0);

		GET_SINGLE(SceneManager)->SetIsGameEnded(false);

		GET_SINGLE(SceneManager)->ChangeScene(SceneType::PLAYSCENE);
		return;
	}

}

void DevScene::Render(ID2D1RenderTarget* renderTarget,float alpha)
{
	Super::Render(renderTarget, alpha);
}
