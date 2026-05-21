#include "pch.h"
#include "GameEngine.h"
#include "Managers.h"	//각종 매니저들 모아둔 헤더파일


GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::Init(HINSTANCE hInst)
{
	

}

void GameEngine::Update()
{
	GET_SINGLE(InputManager)->Update();



}

void GameEngine::Render()
{
}
