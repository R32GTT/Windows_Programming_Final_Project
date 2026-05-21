#include "pch.h"
#include "GameEngine.h"
#include "Managers.h"	//각종 매니저들 모아둔 헤더파일


GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::Init(HWND hWnd)
{
	memhWnd = hWnd;
	memDC = ::GetDC(hWnd);

	GetClientRect(hWnd, &rect);

	memDCDB = ::CreateCompatibleDC(memDC);
	memHBITMAPDB = ::CreateCompatibleBitmap(memDC, rect.right, rect.bottom);
	HBITMAP prev = (HBITMAP)SelectObject(memDCDB, memHBITMAPDB);
	::DeleteObject(prev);

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init(hWnd);
	GET_SINGLE(SceneManager)->init();

}

void GameEngine::Update()
{
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(SceneManager)->Update();


}

void GameEngine::Render()
{
}
