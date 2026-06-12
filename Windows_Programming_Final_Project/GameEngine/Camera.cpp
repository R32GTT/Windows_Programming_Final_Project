#include "pch.h"
#include "Managers.h"
#include "Camera.h"
#include <random>

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

    Vec2F targetPos = playerPos + mouseOffsetFromCenter * lookAhead;

    if (_shakeTimer > 0.0f)
    {
        // 틱이 1/60 고정이므로 명시적으로 상수값을 뺌
        const float FIXED_TICK = 1.0f / 60.0f;
        _shakeTimer -= FIXED_TICK;

        if (_shakeTimer < 0.0f) _shakeTimer = 0.0f;

        float damping = (_shakeDuration > 0.0f) ? (_shakeTimer / _shakeDuration) : 0.0f;

        damping = damping * damping;

        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-_shakeMagnitude, _shakeMagnitude);

        targetPos.x += dist(gen);
        targetPos.y += dist(gen);
    }

    _pos = targetPos;
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
