#pragma once
#include "pch.h"
#include <dwrite.h> // DirectWrite 헤더
#pragma comment(lib, "dwrite.lib")

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

	IDWriteFactory* _dwriteFactory{};
	IDWriteTextFormat* _textFormat{};
	ID2D1SolidColorBrush* _debugBrush{};

	float _accumulator = 0.f;
	float _alpha = 0.f;
	const float _FIXED_DT = 1.0f / 60.0f;
};

