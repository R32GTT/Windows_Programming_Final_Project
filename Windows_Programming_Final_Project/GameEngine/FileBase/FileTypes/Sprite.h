#pragma once
#include "pch.h"
#include "../FileBase.h"

class Texture;


class Sprite : public FileBase
{
public:
	Sprite(Texture* texture, float x, float y, float cx, float cy);
	virtual ~Sprite();

	Texture* GetTexture() { return _texture; }
	ID2D1Bitmap* GetBitmap();


	Vec2F	GetPos() { return Vec2F{_x, _y}; };
	Vec2F	GetSize() { return Vec2F{_cx, _cy}; };

	D2D1_RECT_F GetSrcRect();

private:
	Texture* _texture{};
	float _x{};
	float _y{};
	float _cx{};
	float _cy{};
};