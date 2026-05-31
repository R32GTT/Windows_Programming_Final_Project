#include "pch.h"
#include "GameObject.h"

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

//SetPosition SetSize SetHalfSize 함수들 추가
void GameObject::SetPosition(Vec2<float> _pos) {


}

void GameObject::SetSize(float x, float y) {


}

void GameObject::SetHalfSize(Vec2<float> halfSize) {



}


