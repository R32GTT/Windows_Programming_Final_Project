#include "pch.h"
#include "Projectile.h"
#include <random>
#include "Managers.h"
#include "Weapon.h"

static std::random_device rd;
static std::mt19937 gen(rd());

Projectile::Projectile()
{
	type = OBJECTTYPE::PROJECTILE;
	layer = Layers::ACTORS;
}

Projectile::Projectile(GameObject* owner, WPTYPE wpType)
{
	type = OBJECTTYPE::PROJECTILE;
	layer = Layers::ACTORS;
	WeaponInfo wpInfo = GetWeaponInfo(wpType);
	_projType = wpInfo.__projType;
	_owner = owner;
	_isLethal = wpInfo.isLethal;	// 살상용
	_lifeTime = wpInfo.projectile_Life;            // 최대 생존 시간 (초)
	// 현재 생존 시간 계산용
	pos = prevPos = owner->GetPos();
	speed = wpInfo.projectile_Speed;
	_dir = movingDir = facingDir = owner->GetFacningDir();
	_halfSize = wpInfo._projSize;
	if (wpInfo.__projType == ProjectileType::BULLET)
	{
		Vec2F tfacingDir = owner->GetFacningDir();

		std::normal_distribution<float> dist(0.0f, wpInfo.projectile_Deviation);
		float randomAngle = dist(gen);
		float cosA = std::cos(randomAngle);
		float sinA = std::sin(randomAngle);
		Vec2<float> newDir;
		newDir.x = tfacingDir.x * cosA - tfacingDir.y * sinA;
		newDir.y = tfacingDir.x * sinA + tfacingDir.y * cosA;
		_dir = movingDir = facingDir = newDir.Normalized();

		Vec2F facingDir = owner->GetFacningDir();
		Vec2F rightDir = Vec2F(-facingDir.y, facingDir.x);
		float offsetDistance = 11.0f;
		pos = pos + (rightDir * offsetDistance);
		
	}
	prevPos = pos = pos + _dir * 65.0f;
	_rotationAngle = _rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f; // Sprite가 위를 향하기 때문에 +90
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
	if (_elapsedTime >= _lifeTime)
		_isDead = true;
}

void Projectile::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;
	Vec2F screenPos = GetRenderPos(alpha);

	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(ToRenderPos.x, ToRenderPos.y), _halfSize.x, _halfSize.y);

	ID2D1SolidColorBrush* brush = nullptr;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brush);

	renderTarget->FillEllipse(ellipse, brush);

	if (brush != nullptr)
		brush->Release();

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
