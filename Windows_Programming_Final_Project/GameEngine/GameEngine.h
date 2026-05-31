#pragma once
#include "pch.h"
class GameEngine
{
public:

	GameEngine();
	~GameEngine();

	void Init(HWND hWnd);
	void Update();
	void Render();

private:
	RECT rect;
	HWND memhWnd{};
	HDC memDC{};

	// 더블 버퍼링용
	/*HDC memDCDB{};
	HBITMAP memHBITMAPDB{};*/

	ID2D1Factory* _d2dFactory{};
	ID2D1HwndRenderTarget* _renderTarget{};
	IWICImagingFactory* _wicFactory{};

	float _accumulator = 0.f;
	float _alpha = 0.f;
	const float _FIXED_DT = 1.0f / 60.0f;
};

