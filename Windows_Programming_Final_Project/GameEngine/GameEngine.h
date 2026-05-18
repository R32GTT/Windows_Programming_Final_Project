#pragma once
#include "pch.h"
class GameEngine
{
public:

	GameEngine();
	~GameEngine();

	void Init(HINSTANCE hInst);
	void Update();
	void Render();


	RECT rect;
	HWND memhWnd{};
	HDC memDC{};

	// 더블 버퍼링용
	HDC memDCDB{};
	HBITMAP memHBITMAP{};
};

