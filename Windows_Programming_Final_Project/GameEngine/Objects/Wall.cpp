#include "pch.h"
#include "Wall.h"
#include "Managers.h"

Wall::~Wall()
{
}

void Wall::Init()
{
}

void Wall::Update()
{
}

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
