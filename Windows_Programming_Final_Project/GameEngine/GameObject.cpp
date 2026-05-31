#include "pch.h"
#include "GameObject.h"

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


