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

void GameObject::Render(HDC)
{
}

bool GameObject::CheckDead()
{
	return false;
}
