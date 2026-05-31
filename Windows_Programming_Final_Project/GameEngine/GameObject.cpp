#include "pch.h"
#include "GameObject.h"
#include "Managers.h"

unsigned int GameObject::_sNextId = 1;

GameObject::GameObject()
{
	_id = _sNextId++;

	//오브젝트 고유 번호는 있으니 필요한 것들을 추가함.
	//지워도 괜찮음
	pos = { 0.0f, 0.0f };
	prevPos = pos;

	facingDir = { 1.0f, 0.0f };
	movingDir = { 0.0f, 0.0f };
	_halfSize = { 25.0f, 25.0f };
	speed = 0.0f;


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

void GameObject::Render(ID2D1RenderTarget* renderTarget, float alpha)
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


