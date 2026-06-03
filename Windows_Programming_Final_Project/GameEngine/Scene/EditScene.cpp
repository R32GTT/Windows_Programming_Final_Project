#include "pch.h"
#include "EditScene.h"
#include "Managers.h"
#include "Camera.h"
#include <fstream>
#include "../FileBase/json.hpp"


using json = nlohmann::json;

EditScene::EditScene()
{
	//생성자
	//안전한 기본값만 넣기
	//mouse
	//camera
	//메모리 이미지 DC 펜 브러시 리소스 해제

}

EditScene::~EditScene()
{

	//소멸자
	//Release();
	//예시 커밋하기

}

void EditScene::Init()
{
	//Scene 시작 시 필요한 데이터 준비
	//처음 플레이어 상태 IDLE
	//무기는 처음에는 주먹 
    PlayerState::IDLE;
	//OBJECTTYPE::WEAPON;


}

void EditScene::Update()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_1)) _currentEntity = EntityType::PlayerSpawn;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2)) _currentEntity = EntityType::Enemy;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_3)) _currentEntity = EntityType::Weapon;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_4)) _currentEntity = EntityType::Wall;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_5)) _currentEntity = EntityType::Endpoint;

	POINT curmousePos = GET_SINGLE(InputManager)->GetMousePos();
	Vec2<float> camPos = GET_SINGLE(SceneManager)->GetCameraPos();

	int worldX = (int)curmousePos.x + camPos.x;
	int worldY = (int)curmousePos.y + camPos.y;

	int snapX = (worldX / _gridSize) * _gridSize;
	int snapY = (worldY / _gridSize) * _gridSize;

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::MiddleMouse))
		_prevMousePos = curmousePos;
	else if (GET_SINGLE(InputManager)->GetButton(KeyType::MiddleMouse))
	{
		int dX = curmousePos.x - _prevMousePos.x;
		int dY = curmousePos.y - _prevMousePos.y;

		camPos.x -= dX;
		camPos.y -= dY;

		GET_SINGLE(SceneManager)->SetCameraPos(camPos);

		_prevMousePos = curmousePos;
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
	{

	}
}

void EditScene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Super::Render(renderTarget,alpha);
}

void EditScene::SaveMap(const std::wstring& fileName)
{
	json mapData;
	mapData["objects"] == json::array();

	for (int i = 0; i < (int)Layers::LAYER_COUNT; i++)
	{
		for (GameObject* obj : GetObjectsByLayer(static_cast<Layers>(i)))
		{
			json obJson;
		}
	}


}

void EditScene::LoadMap(const std::wstring& fileName)
{


}


