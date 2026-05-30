#include "pch.h"
#include "Camera.h"

void Camera::Init()
{
}

void Camera::TickComp()
{
    if (_owner == nullptr) return;


    Vec2<float> ownerPos = _owner->GetPos();
}

void Camera::Render(HDC hDC, float alpha)
{

}
