#include "pch.h"
#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

Texture* Texture::LoadBmp(HWND hWnd, const std::wstring& path)
{
	HDC hDC = ::GetDC(hWnd);

	_hDC = ::CreateCompatibleDC(hDC);

	_bitmap = (HBITMAP)::LoadImage(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	if (_bitmap == NULL)
	{
		::MessageBox(hWnd, path.c_str(), L"Image Load Failed!", NULL);
	}

	HBITMAP prev = (HBITMAP)SelectObject(_hDC, _bitmap);
	::DeleteObject(prev);

	BITMAP bit = {};
	::GetObject(_bitmap, sizeof(BITMAP), &bit);
	_size.x = bit.bmWidth;
	_size.y = bit.bmHeight;

	return this;
}

HDC Texture::GetDC()
{
	return _hDC;
}
