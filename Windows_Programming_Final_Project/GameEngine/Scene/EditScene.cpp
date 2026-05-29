#include "pch.h"
#include "EditScene.h"
#include "Managers.h"
#include "Camera.h"
#include <fstream>


EditScene::EditScene()
{
	//생성자
	//안전한 기본값만 넣기
	//mouse
	//camera

}

EditScene::~EditScene()
{

	//소멸자
	//Release();


}

void EditScene::Init()
{
	//Scene 시작 시 필요한 데이터 준비
}

void EditScene::Update()
{
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_1)) _currentEntity = EntityType::PlayerSpawn;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2)) _currentEntity = EntityType::Enemy;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_3)) _currentEntity = EntityType::Weapon;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_4)) _currentEntity = EntityType::Wall;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_5)) _currentEntity = EntityType::Endpoint;

	POINT curmousePos = GET_SINGLE(InputManager)->GetMousePos();
	Vec2<int> camPos = GET_SINGLE(SceneManager)->GetCameraPos();

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

void EditScene::Render(HDC hdc)
{
}
