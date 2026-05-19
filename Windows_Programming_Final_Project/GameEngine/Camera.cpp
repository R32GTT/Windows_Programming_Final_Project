#include "pch.h"
#include "Camera.h"

void Camera::Init()
{
}

void Camera::TickComp()
{
	if (_owner == nullptr)
		return;

	Vec2 pos = _owner->pos;

	

}

void Camera::Render(HDC hDC)
{

}
