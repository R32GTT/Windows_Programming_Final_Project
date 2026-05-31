#include "pch.h"
#include "../FileBase.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(Texture* texture, float x, float y, float cx, float cy)
	: _texture(texture), _x(x), _y(y), _cx(cx), _cy(cy)
{

}

Sprite::~Sprite()
{
}

ID2D1Bitmap* Sprite::GetBitmap()
{
	if (_texture)
		return _texture->GetBitmap();
	return nullptr;
}

D2D1_RECT_F Sprite::GetSrcRect()
{
	return D2D1_RECT_F(_x, _y, _x + _cx, _y + _cy);
}



