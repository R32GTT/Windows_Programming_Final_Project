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
	HDC memDCDB{};
	HBITMAP memHBITMAPDB{};

	float _accumulator = 0.f;
	float _alpha = 0.f;
	const float _FIXED_DT = 1.0f / 60.0f;
};

