#include "pch.h"
#include "Enemy.h"
#include "Managers.h"
#include "../LevelData/LevelData.h"

Enemy::Enemy()
{
	speed = baseEnemySpeed;
	_halfSize = Vec2F(25.0f, 10.0f);

	type = OBJECTTYPE::ENEMY;
	layer = Layers::ACTORS;
}

Enemy::Enemy(EnemyType etype)
{
	_halfSize = Vec2F(25.0f, 10.0f);
	type = OBJECTTYPE::ENEMY;
	layer = Layers::ACTORS;

	if (etype == EnemyType::NORMAL)
		speed = baseEnemySpeed;
	else
		speed = armoredEnemySpeed;
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	FlipBook* idleAnim = GET_SINGLE(FileManager)->GetFlipBook(L"MobCharAnim_Idle");
	FlipBook* idleAnimF = GET_SINGLE(FileManager)->GetFlipBook(L"FMobCharAnim_Idle");
	
	PlayAnimation(idleAnim);
}

void Enemy::Update()
{
	if (IsKilled())	return;


}

void Enemy::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	Vec2F screenPos = GetRenderPos(alpha);

	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	RenderAnimation(renderTarget, ToRenderPos.x, ToRenderPos.y);
}

bool Enemy::IsKilled()
{
	return _enemyState == EnemyState::DEAD;
}

void Enemy::OnCollision(GameObject* other)
{
	if (IsKilled()) return;

	switch (other->GetObjectType())
	{
	case OBJECTTYPE::PROJECTILE:
		_enemyState = EnemyState::DEAD;
	default:
		break;
	}
}

void Enemy::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.enemyType = _enemyType;
}

void Enemy::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
	_enemyType = spawnData.enemyType;
}

void Enemy::EmMove()
{
}

void Enemy::SetEnemyType(EnemyType etype)
{
	_enemyType = etype;
}

EnemyType Enemy::What_Enemy()
{
	if (_enemyType != EnemyType::NORMAL) {
		_enemyType = EnemyType::ARMORED;
		return (_enemyType);
	}
	else {
		return (_enemyType);
	}
}

EnemyType Enemy::GetEType()
{
	return _enemyType;
}
