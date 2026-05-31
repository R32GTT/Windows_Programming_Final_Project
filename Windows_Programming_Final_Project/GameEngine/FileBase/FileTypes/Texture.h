#pragma once
#include "pch.h"
#include "../FileBase.h"

class Texture : public FileBase
{
public:
	Texture();
	virtual ~Texture();

	void Load(ID2D1RenderTarget* renderTarget, IWICImagingFactory* wicFactory, const std::wstring& path);

	ID2D1Bitmap* GetBitmap() { return _bitmap; };

	void		SetSize(Vec2F size) { _size = size; };
	Vec2F	GetSize() { return _size; };


private:
	ID2D1Bitmap* _bitmap{};
	Vec2F		_size{};
	
};