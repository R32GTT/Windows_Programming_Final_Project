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
	prevPos = pos = pos + _dir * 40.0f;
	_rotationAngle = _dir.Angle() * (180.0f / PI);

	_wpType = wpType;

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
	//if (_projType == ProjectileType::MELEE) return;
	Vec2F screenPos = GetRenderPos(alpha);
	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	// 1. 기존 트랜스폼 폼 저장
	D2D1_MATRIX_3X2_F oldTransform;
	renderTarget->GetTransform(&oldTransform);

	// 2. 바라보는 방향(facingDir)을 각도(Degree)로 변환 후 렌더 타겟 회전!
	float angle = atan2(facingDir.y, facingDir.x) * (180.0f / 3.14159265f);
	renderTarget->SetTransform(
		oldTransform * D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(ToRenderPos.x, ToRenderPos.y))
	);

	// 3. OBB는 사각형(Rectangle)이므로 사각형으로 그려보는 것을 추천합니다.
	D2D1_RECT_F rect = D2D1::RectF(ToRenderPos.x - _halfSize.x, ToRenderPos.y - _halfSize.y,
		ToRenderPos.x + _halfSize.x, ToRenderPos.y + _halfSize.y);

	ID2D1SolidColorBrush* brush = nullptr;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow, 0.5f), &brush); // 반투명 노란색
	renderTarget->FillRectangle(rect, brush);
	if (brush) brush->Release();

	// 4. 렌더 타겟 원상복구 (매우 중요!)
	renderTarget->SetTransform(oldTransform);

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
