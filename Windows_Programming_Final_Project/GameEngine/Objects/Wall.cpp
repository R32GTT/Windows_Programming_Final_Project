#include "pch.h"
#include "Wall.h"

Wall::~Wall()
{
}

void Wall::Init()
{
}

void Wall::Update()
{
}

void Wall::Render(HDC hdc, float alpha)
{
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
