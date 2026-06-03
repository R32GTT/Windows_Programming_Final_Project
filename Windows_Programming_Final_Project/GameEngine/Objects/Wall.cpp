#include "pch.h"
#include "Wall.h"
#include "GameObject.h"

Wall::~Wall()
{
}

//wall layer를 수정
//halfsize 일단 25 25로 함
void Wall::Init()
{
	layer = Layers::WALL;
	_halfSize = { 25.0f, 25.0f };

}

void Wall::Update()
{
}

//Wall Render 시작하기
//Sprite가 없으니 일반 도형으로 Render하기
void Wall::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (renderTarget == nullptr) return;

	Vec2<float> renderPos = GetRenderPos(alpha);

	D2D1_RECT_F rect = D2D1::RectF(renderPos.x - _halfSize.x, renderPos.y - _halfSize.y, renderPos.x + _halfSize.x, renderPos.y + _halfSize.y);

	ID2D1SolidColorBrush* brush = nullptr;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), &brush);

	if (brush != nullptr)
	{
		renderTarget->FillRectangle(rect, brush);
		brush->Release();
	}

}

Wall::Wall(POINT start, POINT end)
{
	type = OBJECTTYPE::WALL;
	layer = Layers::WALL;

	wallCoords.left = start.x; wallCoords.top = start.y;
	wallCoords.right = end.x; wallCoords.bottom = end.y;

	Vec2F startV(start);
	Vec2F endV(end);

	pos = (startV - endV) / 2.0f;

	_halfSize = (endV - startV).Abs() / 2.0f;

	prevPos = pos;

}
