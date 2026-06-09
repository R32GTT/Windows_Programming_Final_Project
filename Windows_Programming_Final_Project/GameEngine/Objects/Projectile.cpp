#include "pch.h"
#include "Projectile.h"

Projectile::Projectile()
{
	type = OBJECTTYPE::PROJECTILE;
	layer = Layers::ACTORS;
}

Projectile::~Projectile()
{
}

//Projectile.cpp 함수들 추가
void Projectile::Init() {
}

void Projectile::Update()
{
	//60fps라고 가정하고 고정 시간으로 움직이는 방식
	float deltaTime = 0.016f;

	prevPos = pos;

	pos = pos + _dir * speed * deltaTime;

	_elapsedTime += deltaTime;

}

void Projectile::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	Vec2<float> renderPos = GetRenderPos(alpha);

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(renderPos.x, renderPos.y), _halfSize.x, _halfSize.y);

	ID2D1SolidColorBrush* brush = nullptr;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brush);

	renderTarget->FillEllipse(ellipse, brush);

	if (brush != nullptr)
		brush->Release();

}

bool Projectile::CheckDead()
{
	return _elapsedTime >= _lifeTime;
}

void Projectile::OnCollision(GameObject* other)
{
	if (_isDead) return;

	OBJECTTYPE otherType = other->GetObjectType();


	if (otherType == OBJECTTYPE::WALL)
	{
		_isDead = true;
	}

	else if (otherType == OBJECTTYPE::ENEMY || otherType == OBJECTTYPE::PLAYER)
	{
		_isDead = true;
	}
}

void Projectile::SetInfo(GameObject* owner, ProjectileType type, float damage, float speed, float lifeTime)
{
}

void Projectile::SetDirection(Vec2<float> dir)
{
	_dir = dir.Normalized();
}

void Projectile::SetOwner(GameObject* owner)
{
	_owner = owner;
}

void Projectile::SaveToData(ObjectSpawnData& outData)
{
}

void Projectile::LoadFromData(const ObjectSpawnData& spawnData)
{
}
