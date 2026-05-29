#include "pch.h"
#include "EditScene.h"
#include "Managers.h"
#include "Camera.h"
#include <fstream>
#include<stdio.h>

//맵 저장하는 함수
void EditScene::SaveMap(const char* fileName) {

	FILE* fp = fopen(fileName, "w");

	if (fp == NULL) {
		printf("Save Fail.\n");
		return;
	}


}



//맵 불러오는 함수
void EditScene::LoadMap(const char* fileName) {

	FILE* fp = fopen(fileName, "r");

	if (fp == NULL) {
		printf("Load Fail.\n");
		return;
	}


}


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
	//처음 플레이어 상태 IDLE
	//무기는 처음에는 주먹 
    //PlayerState::IDLE;
	


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

void EditScene::Render(HDC hdc, float alpha)
{
	//Render에서 필요한 것은???

	//배경 그리기
	//배치된 오브젝트 그리기
	//선택된 오브젝트 표시


}
