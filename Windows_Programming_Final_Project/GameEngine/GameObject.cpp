#include "pch.h"
#include "GameObject.h"
#include "Managers.h"

unsigned int GameObject::_sNextId = 1;

GameObject::GameObject()
{
	_id = _sNextId++;
}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
}

void GameObject::Update()
{
}

void GameObject::Render(HDC hdc, float alpha)
{
}



bool GameObject::CheckDead()
{
	return false;
}

Vec2<float> GameObject::GetRenderPos(float alpha)
{
	return prevPos * (1.0f - alpha) + pos * alpha;
}

Vec2I GameObject::GetScreenPos(float alpha)
{
    Vec2<float> worldPos = GetRenderPos(alpha);

    Vec2<float> camPos = GET_SINGLE(SceneManager)->GetCameraPos();

    int screenX = std::round(worldPos.x - camPos.x + WinSizeX / 2.0f);
    int screenY = std::round(worldPos.y - camPos.y + WinSizeY / 2.0f);

    return { screenX, screenY };
}


