#include "pch.h"
#include "Managers.h"
#include "Camera.h"

void Camera::Init()
{
}

void Camera::TickComp()
{
    if (_owner == nullptr) return;

    Vec2<float> playerPos = _owner->GetPos();
    Vec2<float> screenMousePos(GET_SINGLE(InputManager)->GetMousePos());
    Vec2<float> halfWinSize(WinSizeX / 2.0f, WinSizeY / 2.0f);

    Vec2<float> mouseOffsetFromCenter = screenMousePos - halfWinSize;

    float lookAhead = 0.2f;

    _pos = playerPos + mouseOffsetFromCenter * lookAhead;
}

void Camera::Render(HDC hDC, float alpha)
{

}

void Camera::SetOwner(GameObject* P)
{
     _owner = P;

     if (_owner == nullptr)
         return;

     _pos = _owner->GetPos();
     _prevPos = _pos;
}
