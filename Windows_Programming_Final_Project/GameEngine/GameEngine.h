#pragma once
#include "../pch.h"
class GameEngine
{
public:

	GameEngine();
	~GameEngine();

	void Init(HWND hWnd);


	HWND memhWnd;
	HDC memDC;
	HBITMAP memHBITMAP;


};

