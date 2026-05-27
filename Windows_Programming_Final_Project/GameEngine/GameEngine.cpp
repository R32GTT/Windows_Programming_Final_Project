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

	GET_SINGLE(SceneManager)->ChangeScene(SceneType::DEVSCENE);

}

void GameEngine::Update()
{

	
	GET_SINGLE(TimeManager)->Update();
	float dt = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (dt > 0.25f)
		dt = 0.25f;

	_accumulator += dt;

	while (_accumulator >= _FIXED_DT)
	{

		GET_SINGLE(InputManager)->Update();
		GET_SINGLE(SceneManager)->Update();

		_accumulator -= _FIXED_DT;
	}

}

void GameEngine::Render()
{
	float alpha = _accumulator / _FIXED_DT;

	GET_SINGLE(SceneManager)->Render(memDCDB);

	unsigned int fps = GET_SINGLE(TimeManager)->GetFps();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	{
		std::wstring str = std::format(L"FPS({0}), DT({1})", fps, deltaTime);
		::TextOut(memDCDB, 550, 10, str.c_str(), static_cast<int>(str.size()));
	}

	::BitBlt(memDC, 0, 0, rect.right, rect.bottom, memDCDB, 0, 0, SRCCOPY); // 비트 블릿 : 고속 복사
	::PatBlt(memDCDB, 0, 0, rect.right, rect.bottom, WHITENESS);
}
