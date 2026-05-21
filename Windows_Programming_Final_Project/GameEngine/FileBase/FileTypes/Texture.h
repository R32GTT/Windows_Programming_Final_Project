#pragma once
#include "pch.h"
#include "../FileBase.h"

class Texture
{
public:
	Texture();
	virtual ~Texture();

	Texture*	LoadBmp(HWND hWnd, const std::wstring& path);
	HDC			GetDC();

	void		SetSize(Vec2<int> size) { _size = size; };


private:
	HDC				_hDC{};
	HBITMAP			_bitmap{};
	Vec2<int>		_size{};
	unsigned int	_transparent = RGB(255, 0, 255);
};