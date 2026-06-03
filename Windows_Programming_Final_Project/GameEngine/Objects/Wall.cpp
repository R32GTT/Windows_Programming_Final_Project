#include "pch.h"
#include "Wall.h"
#include "Managers.h"

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
	if (!renderTarget) return;

	Vec2F screenPos = GetRenderPos(alpha);

	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	RenderAnimation(renderTarget, ToRenderPos.x, ToRenderPos.y);

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
