#pragma once
#include "pch.h"
#include "../FileBase.h"

class Texture;


class Sprite : public FileBase
{
public:
	Sprite(Texture* texture, int x, int y, int cx, int cy);
	virtual ~Sprite();

	HDC			GetDC();
	int			GetTransparent();
	Vec2<int>	GetPos() { return Vec2<int>{_x, _y}; };
	Vec2<int>	GetSize() { return Vec2<int>{_cx, _cy}; };

private:
	Texture* _texture{};
	int _x{};
	int _y{};
	int _cx{};
	int _cy{};
};